#include "multi_file_handler.h"

MultiFileHandler::MultiFileHandler() {
}

MultiFileHandler::~MultiFileHandler() {
}

void MultiFileHandler::addFile(const std::string& path) {
    files.push_back(path);
}

void MultiFileHandler::clearFiles() {
    files.clear();
}

const std::vector<std::string>& MultiFileHandler::getFiles() const {
    return files;
}

bool MultiFileHandler::processFiles(std::function<bool(const std::string&)> processor) {
    for (const auto& file : files) {
        if (!processor(file)) {
            return false;
        }
    }
    return true;
}