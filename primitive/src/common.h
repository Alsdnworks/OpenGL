#ifndef __COMMON_H__
#define __COMMON_H__
#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define CLASS_PTR(klassName)                             \
     class klassName;                                    \
     using klassName##UPtr = std::unique_ptr<klassName>; \
     using klassName##Ptr = std::shared_ptr<klassName>;  \
     using klassName##WPtr = std::weak_ptr<klassName>;
std::optional<std::string> LoadTextFile(const std::string &filename);

#endif

/*h가 디파인이 되어있지않는다면?-중복사용시 오류를 방지한다(함수2회선언)
#ifndef __COMMON_H__

 
#define __COMMON_H__1

내용
LoadTextFile이라는 함수 생성(const std::string& filename)여기는 인자

#endif->여기까지 ifndef범위다.


 
*/