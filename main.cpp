#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>

bool isEqual(const char* str1, const char* str2){
  if (str1 == nullptr || str2 == nullptr) return false;
  while(*str1 != '\0' && *str2 != '\0'){
    if(!isalnum(*str1) || !isalnum(*str2)) return false;
    if(tolower(*str1) != tolower(*str2)) return false;
    str1++;
    str2++;
  }
  return (*str1 == '\0' && *str2 == '\0');
}

void processBlock(char** uniqueWords, int* wordCount, int& numUniqueWords, char* partialWord, int& partialWordLen, const char* block, int blockSize, const int maxUniqueWords){
  const int maxWordLength = 100;
  char currentWord[maxWordLength];
  int index = 0;
  while(index < blockSize){
    int wordIndex = 0;
    while(index < blockSize && !isalnum(block[index])){
      index++;
    }
    while(index < blockSize && isalnum(block[index])){
      currentWord[wordIndex++] = block[index++];
      if (wordIndex == maxWordLength - 1){ 
        while (index < blockSize && isalnum(block[index])){
          currentWord[maxWordLength - 2] = block[index++];
        }
        currentWord[maxWordLength - 1] = '\0';
        if (partialWordLen > 0) {
          strncat(currentWord, partialWord, partialWordLen);
          partialWord[0] = '\0';
          partialWordLen = 0;
        }
        break;
      }
    }
    currentWord[wordIndex] = '\0';
    if(wordIndex == 0) continue;
    if(partialWordLen > 0){
      strncat(currentWord, partialWord, partialWordLen);
      partialWord[0] = '\0';
      partialWordLen = 0;
    }
    bool found = false;
    for(int i = 0; i < numUniqueWords; i++){
      if(isEqual(uniqueWords[i], currentWord)){
        wordCount[i]++;
        found = true;
        break;
      }
    }
    if(!found){
      if (numUniqueWords >= maxUniqueWords) {
          std::cout << "Array of unique words is full\n";
          return;
      }
      uniqueWords[numUniqueWords] = new char[strlen(currentWord) + 1];
      strcpy(uniqueWords[numUniqueWords], currentWord);
      wordCount[numUniqueWords] = 1;
      numUniqueWords++;
    }
  }
  if (index == blockSize) {
    partialWordLen = maxWordLength - 1;
    strncpy(partialWord, &(currentWord[maxWordLength - 2]), partialWordLen + 1);
  }
  else {
    partialWordLen = 0;
  }
}

int main(){
  std::ifstream file("file.txt", std::ios::binary);
  if(!file){
    std::cout << "Failed to open file\n";
    return -1;
  }
  const int blockSize = 4096;
  const int maxWordLength = 100;
  const int maxUniqueWords = 10000;
  char** uniqueWords = new char*[maxUniqueWords];
  int* wordCount = new int[maxUniqueWords];
  int numUniqueWords = 0;
  char partialWord[maxWordLength];
  int partialWordLen = 0;
  char block[blockSize];
  while(file.read(block, blockSize)){
    processBlock(uniqueWords, wordCount, numUniqueWords, partialWord, partialWordLen, block, blockSize, maxUniqueWords);
  }
  int remainingBytes = file.gcount();
  if(remainingBytes > 0){
    processBlock(uniqueWords, wordCount, numUniqueWords, partialWord, partialWordLen, block, remainingBytes, maxUniqueWords);
  }
  file.close();
  std::cout << "Duplicate words found:\n";
  for(int i = 0; i < numUniqueWords; i++){
   if(wordCount[i] > 1){
      std::cout << uniqueWords[i] << " (" << wordCount[i] << " times)\n";
    }
    delete[] uniqueWords[i];
  }
  delete[] uniqueWords;
  delete[] wordCount;
  return 0;
}