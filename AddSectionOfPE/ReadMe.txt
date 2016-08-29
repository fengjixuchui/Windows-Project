1.update SizeOfHeader in OptionalHeader
2.add the number of section in FileHeader
3.update EP in OptionalHeader to point the new section
4.updata SizeOfImage in OptionalHeader
5.Add a IMAGE_SECTION_HEADER 
6.don't forget clear IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT because it sits behind the last IMAGE_SECTION_HEADER struct , if we add a IMAGE_SECTION_HEADER,we will over the IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT, and sometime will be wrong.
