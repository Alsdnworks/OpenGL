# ExternalProject 관련 명령어 셋 추가
# include는 추가적인 기능을 사용한다는 선언
# 20번째줄의 익스터널을 사용한다고 선언
include(ExternalProject)

# Dependency 관련 변수 설정
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

# spdlog: fast logger library
#21번째줄은 깃헙 주소를 받아서 그것에대해 씨메이크를 실행
ExternalProject_Add(
    #변수설정과같다 컴파일을 할때의 대표이름
    dep_spdlog
    #사용할 라이브러리의 깃주소
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    #버전을 설정한다. 해당 라이브러리 좌측상단에 브랜치와 태그정보를
    #리스트한 태그를 불러 올 수있다.
    GIT_TAG "v1.x"
    #1은 부울값의 스위치로 깃 수ㅖㄹ로우는 코드변경 내역을 모두받지않고
    #최신 커밋만 받을수있다
    GIT_SHALLOW 1
    #깃 레포의 클론으로 다운받고 일련의 과정(빌드의 과정)은
    #업데이트와 패치스텝은 없음,
    #업데이트는 깃풀-(업데이트가된다면...)
    #패치는 디폴트가없고 명시적으로 기재해준것
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    #씨메이크 컴피규어를 위한 인자 -D는 정의(데피니션)
    #CMAKE_INSTALL_PREFIX씨메가 빌드를 했을때 어디에 인스톨 할지
    #${DEP_INSTALL_DIR}에 지정한 set변수를 통해서...15번째줄로
    #인스톨 디렉터리를 만들어 결과물을 거기로 넣겠다~라는뜻
    #이해가 안가면 17분부터보자
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
    #
    TEST_COMMAND ""
)
# Dependency 리스트 및 라이브러리 파일 리스트 추가
set(DEP_LIST ${DEP_LIST} dep_spdlog)
#26분 참조... 이해안감
#디버그로 컨피그시 d를 붙여줌 install/lib/spdlogd.txt
set(DEP_LIBS ${DEP_LIBS} spdlog$<$<CONFIG:Debug>:d>)
#이하 마찬가지

# glfw는 opengl로 작성한 코드를 응용하는데 사용된다
ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.3.2"
    GIT_SHALLOW 1
    UPDATE_COMMAND "" 
    PATCH_COMMAND "" 
    TEST_COMMAND ""
    CMAKE_ARGS
    #인스톨경로 이것도 스피드로그와 마찬가지
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        #glfw의 예제,시험파일,각종문서는 안쓸께요
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
        #glfw깃에서 ㅡcmakelist를 보면 이런 옵션이있다
        #그 내용은 위 세줄을 빌드할지 켜고 끌수있다(on/off)
        #D가 뭔지는 여쭤보고싶
    )
    #라이브러리와 리스트를 설정
    #자세한건 씨메이크.txt참고
set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)