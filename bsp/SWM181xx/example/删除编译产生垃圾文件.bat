copy Objects\ExecutableFile.hex ExecutableFile.hex
del /Q Listings\*.*
del /Q out\*.*
del /Q Objects\*.__i
del /Q Objects\*.crf
del /Q Objects\*.d
del /Q Objects\*.o
del /Q Objects\*._ia
del /Q *.txt
del /Q *.ini
del /Q *.bak
del /Q *.plg
del /Q *.dep
del /Q *.Administrator
rmdir /s/q DebugConfig
exit