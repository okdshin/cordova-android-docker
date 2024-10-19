FROM ubuntu:24.04

# Common setup
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    default-jdk \
    nodejs \
    npm \
    unzip \
    wget

# Setup Android SDK
RUN wget https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip
RUN unzip commandlinetools-linux-11076708_latest.zip
RUN mkdir /android_sdk && \
    (yes | /cmdline-tools/bin/sdkmanager --sdk_root=/android_sdk --install "build-tools;34.0.0")
ENV ANDROID_HOME=/android_sdk

# Setup Gradle
RUN wget https://services.gradle.org/distributions/gradle-8.7-bin.zip
RUN unzip gradle-8.7-bin.zip && ln -s /gradle-8.7/bin/gradle /bin/gradle

# Setup Cordova
RUN npm install -g cordova \
    && (no | cordova)  # disable telemetry
RUN cd /tmp && cordova create tmpApp && cd tmpApp && cordova platform add android && cordova build android
