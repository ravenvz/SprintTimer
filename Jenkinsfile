node {
    stage("Checking out source code") {
        parallel "linux checkout":{
            node ("linux") {
                deleteDir();
                checkout scm;
            }
        }, "windows checkout":{
            node ("windows") {
                deleteDir();
                checkout scm;
            }
        }
    }
    stage('Build') {
        parallel "linux build":{
            node ("linux") {
                sh "mkdir build && cd build && cmake -DCMAKE_VERBOSE_MAKEFILE=OFF -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON .. && make -j\$(nproc)"
            }
        }, "windows build":{
            node ("windows") {
                // TODO clean this up
                bat 'mkdir build && cd build && cmake .. -G "Visual Studio 15 Win64" -DCMAKE_VERBOSE_MAKEFILE="OFF" -DCMAKE_PREFIX_PATH="J:/Qt/5.11.0/msvc2017_64/lib/cmake" -DCMAKE_INCLUDE_PATH="C:/Users/Pavel/Downloads/boost_1_67_0/boost_1_67_0" -DBUILD_TESTS=ON -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE -DBUILD_SHARED_LIBS=TRUE && cmake --build . --config Release'
            }
        }
    }
    stage('Run tests') {
        parallel "linux unit tests":{
            node ("linux") {
                sh 'cd bin && ./test_core --gtest_output=xml:test_core_results.xml && ./test_qt_storage_impl --gtest_output=xml:test_qt_storage_impl_results.xml && mkdir -p test_results && mv *.xml test_results'
                junit '**/bin/test_results/*.xml'
            }
        }, "windows unit tests":{
            node ("windows") {
                bat 'cd bin/Release && test_core.exe --gtest_output=xml:test_core_results.xml && test_qt_storage_impl.exe --gtest_output=xml:test_qt_storage_impl_results.xml && mkdir test_results && move *.xml test_results'
                junit '**/bin/Release/test_results/*.xml'
            }
        }
    }
    stage('Archive binaries') {
        parallel "linux archive":{
            node ("linux") {
                archiveArtifacts '**/lib/*.so'
                archiveArtifacts '**/bin/*'
            }
        }, "windows archive":{
            node ("windows") {
                archiveArtifacts '**/bin/Release/*'
            }
        }
    }
}
