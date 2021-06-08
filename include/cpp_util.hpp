#ifndef CPP_UTIL_HPP
#define CPP_UTIL_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cassert>


template<class Type>
void print_arr(Type *arr, size_t count){
    for (size_t i = 0; i < count; i++)
    {
        if(i!=0) std::cout<< ", ";
        std::cout<< arr[i];
    }    
    std::cout<<"\n";
}


template<class Type>
inline void copy_array( const Type *arr_from, Type *arr_to, size_t count){
    for (size_t i = 0; i < count; i++)
        *(arr_to + i) = *(arr_from + i);
}

inline void print_multi_dim_arr(int *array, int height, int width){
	for (int row = 0; row < height; ++row)
	{
		for (int column = 0; column < width; ++column)
		{
			printf("%d,", *(array + (row * width + column)));
		}
		printf("\n");
	}
}

inline int WriteStringToFile(const std::string &file_path, const std::string &content)
{ 
    std::fstream file(file_path, std::ios::out | std::ios::trunc);

    if (!file.good())
    {
        std::cout<<"ERROR accessing file \n";
        return -1;
    }

    file << content;
    // file1.close();
    return 0;
}

inline std::string ReadStringFromFile(std::string file_path)
{ 
    std::fstream file(file_path); // , file.in);

    if (!file.is_open() && !file.good())
    {
        std::cout<<"ERROR accessing file \n";
        return "";
    }

    // std::ostringstream content_buffer (std::ostringstream::ate);  
    std::stringstream content_buffer;
    content_buffer << file.rdbuf();
    
    return content_buffer.str();
}

inline std::string ReadStringFromFile_Fast(std::string file_path)
{
    std::fstream file(file_path);

     assert(file.is_open() && file.good() && "ERROR accessing file");
//     if (!file.is_open() && !file.good())
//     {
//         std::cout<<"ERROR accessing file \n";
//         return "";
//     }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    std::string content_buffer(size, ' ');
    file.seekg(0);
    file.read(&content_buffer[0], size);
    
    return content_buffer;
}

//// Load the file efficiently ?!
//std::string ReadFileAtOnce(fs::path file_path)
//{
//	//input stream to lock the file
//	std::ifstream instream(file_path, std::ios::in | std::ios::binary);
//	const auto file_size = fs::file_size(file_path);
//	std::string buffer(file_size, '\0');
//	instream.read(buffer.data(), file_size);
//	return buffer;
//}
     

inline bool IsDigit(char ch) // https://en.cppreference.com/w/cpp/string/byte/isdigit
{
  return std::isdigit(static_cast<unsigned char>(ch));
}

inline bool IsPositiveNumber(char value[]) {
  for (int i = 0; value[i] != 0; i++) {
    if (!IsDigit(value[i]))
      return false;
  }
  return true;
}

#endif