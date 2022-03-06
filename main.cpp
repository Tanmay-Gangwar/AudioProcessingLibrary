#include<iostream>
#include"audio.hpp"

void help(){
    std::cout << "Valid inputs:\n";
    std::cout << "To find top probabilites for an audiofile:\n";
    std::cout << "./yourcode.out pathToExtractedAudioFile.txt outputFile.txt\n";
    std::cout << "For help:\n";
    std::cout << "./yourcode.out help\n";
}

void processSound(const char *audioFile, const char* outputFile){
    std::vector<std::string> words = {
        "silence", "unknown", "yes", "no", "up", "down", "left", "right", "on", "off", "stop", "go"
    };

    pred_t result[3];
    libaudioAPI(audioFile, result);
    freopen(outputFile, "a", stdout);
    std::cout << audioFile << " ";
    for (int i = 0; i < 3; i++) std::cout << words[result[i].label] << " ";
    for (int i = 0; i < 3; i++) std::cout << result[i].prob << " ";
    std::cout << "\n";
}

int main(int argCount, char **args){
    if (argCount == 1) std::invalid_argument("FOR HELP TYPE \n ./yourcode.out help");
    std::string s = args[1];
    if (s == "help") help();
    else if (argCount == 3) processSound(args[1], args[2]);
    else throw std::invalid_argument("FOR HELP TYPE \n ./yourcode.out help");
    return 0;
}