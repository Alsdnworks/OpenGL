#ifndef __COMMON_H__
#define __COMMON_H__

#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>
     //어떤값이있거나 없는가를 포인터없이 표시가능
     //std::string*로 리턴할시 빈파일이면 널포인터를 반환한다.
     //다만 동작할당과 반환이 귀찮아지므로 이런타입을 사용
std::optional<std::string> LoadTextFile(const std::string& filename);

#endif // __COMMON_H__

/*h가 디파인이 되어있지않는다면?-중복사용시 오류를 방지한다(함수2회선언)
#ifndef __COMMON_H__

//h를 디파인하라
#define __COMMON_H__1

내용
LoadTextFile이라는 함수 생성(const std::string& filename)여기는 인자

#endif->여기까지 ifndef범위다.


//윈도우 환경에서는 이 세가지를 #pragma once; 로 대체가능
*/