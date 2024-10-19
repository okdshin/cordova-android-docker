#include <jni.h>
#include <android/log.h>
#include <string>

#include <ostream>
#include <vector>

#include <iostream>

#include <android/log.h>

//#include <impl.cpp>


namespace {

class android_log_streambuf : public std::streambuf {
protected:
    std::streamsize xsputn(const char_type* s, std::streamsize n) override 
    { 
        /*
        if((index_ + n) > buffer_.size()) {
            std::string str(buffer_.data(), index_);
            __android_log_write(ANDROID_LOG_ERROR, "ndk_echo", str.c_str());
            index_ = 0;
        }
        assert(index_ + n < buffer_.size());
        std::copy(s, s+n, buffer_.begin() + index_);
        index_ += n;
        if(buffer_[index_-1] == '\n') {
            std::string str(buffer_.data(), index_);
            __android_log_write(ANDROID_LOG_ERROR, "ndk_echo", str.c_str());
            index_ = 0;
        }
        */
        std::string str(s, n);
        __android_log_write(ANDROID_LOG_ERROR, "ndk_echo", str.c_str());
        index_ = 0;
        return n;
    };

    int_type overflow(int_type ch) override 
    { 
        __android_log_write(ANDROID_LOG_ERROR, "ndk_echo", std::string(1, static_cast<char>(ch)).c_str());
        return 1;
    }

    /*
    int sync() override {
        std::string str(buffer_.data(), index_);
        __android_log_write(ANDROID_LOG_ERROR, "ndk_echo", str.c_str());
        //__android_log_print(ANDROID_LOG_ERROR, "ndk_echo", "%s", str.c_str());
        index_ = 0;
        return 0;
    }
    */
private:
    size_t index_ = 0;
    std::vector<char> buffer_ = std::vector<char>(256);
};

android_log_streambuf* global_android_log_streambuf() {
    static android_log_streambuf buf_ = android_log_streambuf{};
    return &buf_;
}

} // namespace

namespace {
std::string jstring2string(JNIEnv *env, jstring jstr) {
    if (!jstr) {
        return "";
    }

    const jclass stringClass = env->GetObjectClass(jstr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jstr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = static_cast<size_t>(env->GetArrayLength(stringJbytes));
    jbyte* p_bytes = env->GetByteArrayElements(stringJbytes, nullptr);

    std::string ret = std::string((char *)(p_bytes), length);
    env->ReleaseByteArrayElements(stringJbytes, p_bytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}
} // namespace


#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL
Java_com_rs_my_testapp_plugin_NdkEcho_echo(
        JNIEnv *env,
        jobject /* this */,
        jstring j_input) {
    try {
        std::cerr << "start echo" << std::endl;
        std::string hello = jstring2string(env, j_input) + "Hi! Hello from C++.";
        std::cerr << "finish echo" << std::endl;
        return env->NewStringUTF(hello.c_str());
    }
    catch(std::exception const& e) {
        std::cerr << e.what() << std::endl;
        return env->NewStringUTF("");
    }
}

#ifdef __cplusplus
}
#endif
