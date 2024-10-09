#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <queue>
#include <windows.h>

struct SharedData {
    HANDLE mutex;
    std::queue<std::string> dataQueue;
};

void simulatedWork(int delayFactor) {
    int delay = rand() % delayFactor + 100;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

void sourceProcess(const std::string& inputFile, SharedData* shared) {
    std::ifstream inFile(inputFile);
    std::string line;
    
    while (std::getline(inFile, line)) {
        WaitForSingleObject(shared->mutex, INFINITE);
        shared->dataQueue.push(line);
        ReleaseMutex(shared->mutex);
        simulatedWork(500);
    }
    
    inFile.close();
}

// Level 1: Sink process
void sinkProcess(const std::string& outputFile, SharedData* shared) {
    std::ofstream outFile(outputFile);
    bool running = true;
    
    while (running) {
        WaitForSingleObject(shared->mutex, INFINITE);
        if (!shared->dataQueue.empty()) {
            std::string data = shared->dataQueue.front();
            shared->dataQueue.pop();
            ReleaseMutex(shared->mutex);
            
            outFile << data << std::endl;
            simulatedWork(300);
        } else {
            ReleaseMutex(shared->mutex);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    outFile.close();
}

// Level 2: Filter process
void filterProcess(SharedData* inputShared, SharedData* outputShared) {
    bool running = true;
    
    while (running) {
        WaitForSingleObject(inputShared->mutex, INFINITE);
        if (!inputShared->dataQueue.empty()) {
            std::string data = inputShared->dataQueue.front();
            inputShared->dataQueue.pop();
            ReleaseMutex(inputShared->mutex);
            
            // Simple filter: convert to uppercase
            for (char& c : data) {
                c = toupper(c);
            }
            
            WaitForSingleObject(outputShared->mutex, INFINITE);
            outputShared->dataQueue.push(data);
            ReleaseMutex(outputShared->mutex);
            
            simulatedWork(400);
        } else {
            ReleaseMutex(inputShared->mutex);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

// Level 3: Client process
void clientProcess(const std::string& filename, SharedData* shared) {
    WaitForSingleObject(shared->mutex, INFINITE);
    shared->dataQueue.push(filename);
    ReleaseMutex(shared->mutex);
    
    // Wait for response
    bool received = false;
    std::string response;
    
    while (!received) {
        WaitForSingleObject(shared->mutex, INFINITE);
        if (!shared->dataQueue.empty()) {
            response = shared->dataQueue.front();
            shared->dataQueue.pop();
            received = true;
        }
        ReleaseMutex(shared->mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Response from server: " << response << std::endl;
}

// Level 3: Server process
void serverProcess(SharedData* shared) {
    bool running = true;
    
    while (running) {
        WaitForSingleObject(shared->mutex, INFINITE);
        if (!shared->dataQueue.empty()) {
            std::string filename = shared->dataQueue.front();
            shared->dataQueue.pop();
            ReleaseMutex(shared->mutex);
            
            // Try to open the file
            std::ifstream file(filename);
            std::string response;
            
            if (file.is_open()) {
                std::string content((std::istreambuf_iterator<char>(file)),
                                   std::istreambuf_iterator<char>());
                response = content;
                file.close();
            } else {
                response = "Error: Could not open file";
            }
            
            WaitForSingleObject(shared->mutex, INFINITE);
            shared->dataQueue.push(response);
            ReleaseMutex(shared->mutex);
            
            simulatedWork(300);
        } else {
            ReleaseMutex(shared->mutex);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

int main() {
    srand(1234);
    
    SharedData sourceToFilter;
    SharedData filterToSink;
    sourceToFilter.mutex = CreateMutex(NULL, FALSE, NULL);
    filterToSink.mutex = CreateMutex(NULL, FALSE, NULL);
    
    // Level 1
    std::thread source(sourceProcess, "input.txt", &sourceToFilter);
    std::thread sink(sinkProcess, "output.txt", &filterToSink);
    
    // Level 2
    std::thread filter(filterProcess, &sourceToFilter, &filterToSink);
    
    // Level 3
    SharedData clientServer;
    clientServer.mutex = CreateMutex(NULL, FALSE, NULL);
    std::thread server(serverProcess, &clientServer);
    std::thread client(clientProcess, "test.txt", &clientServer);
    
    // Join threads
    source.join();
    filter.join();
    sink.join();
    server.join();
    client.join();
    
    // Cleanup
    CloseHandle(sourceToFilter.mutex);
    CloseHandle(filterToSink.mutex);
    CloseHandle(clientServer.mutex);
    
    return 0;
}