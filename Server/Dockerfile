FROM ubuntu:24.04 AS build
RUN apt-get update && apt-get install -y --no-install-recommends cmake g++ libasound2-dev libx11-dev libxext-dev libxinerama-dev libxrandr-dev libxcursor-dev libfreetype-dev libfontconfig-dev libgl1-mesa-dev libcurl4-openssl-dev
WORKDIR /src
COPY . .
RUN cmake -S . -B build-cloud -DJUCE_DIR=/src/ThirdParty/JUCE -DNOVA_BUILD_GUI_TESTS=ON -DCMAKE_BUILD_TYPE=Release && cmake --build build-cloud --target nova_cloud_worker --parallel 2
FROM ubuntu:24.04
RUN apt-get update && apt-get install -y --no-install-recommends python3 xvfb xauth libasound2t64 libfreetype6 libfontconfig1 libx11-6 libxext6 libxinerama1 libxrandr2 libxcursor1 libcurl4t64 && rm -rf /var/lib/apt/lists/* && useradd -m nova
COPY --from=build /src/build-cloud/nova_cloud_worker /app/worker
COPY Server/service.py /app/service.py
USER nova
EXPOSE 8080
CMD ["xvfb-run", "-a", "python3", "/app/service.py"]
