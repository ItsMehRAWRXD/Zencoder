#ifndef MULTI_FILE_HANDLER_H
#define MULTI_FILE_HANDLER_H

#include <string>
#include <vector>
#include <functional>

class MultiFileHandler {
public:
    MultiFileHandler();
    ~MultiFileHandler();
    
    void addFile(const std::string& path);
    void clearFiles();
    
    const std::vector<std::string>& getFiles() const;
    
    bool processFiles(std::function<bool(const std::string&)> processor);
    
private:
    std::vector<std::string> files;
};

#endif // MULTI_FILE_HANDLER_H