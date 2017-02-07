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
    stage('Run static analysis') {
        node ("linux") {
            sh "cppcheck --xml --xml-version=2 . 2> check.xml"
        }
        node ("windows") {

        }
    }
    stage('Build') {
        parallel "linux build":{
            node ("linux") {
                sh "cd build && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=On .. && make -j\$(nproc)"
            }
        }, "windows build":{
            node ("windows") {
                // TODO clean this up
                bat 'cd build && cmake .. -G "Visual Studio 14 Win64" -DCMAKE_PREFIX_PATH="C:/Qt/5.7/msvc2015_64" -DCMAKE_INCLUDE_PATH="C:/Users/Pavel/Downloads/boost_1_62_0/boost_1_62_0" -DBUILD_TESTS=ON -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE -DBUILD_SHARED_LIBS=TRUE && cmake --build . --config Release'
            }
        }
    }
    stage('Run tests') {
        parallel "linux unit tests":{
            node ("linux") {
                sh 'cd bin && ./test_core -o junit && ./test_qt_storage_impl -o junit && mkdir -p test_results && mv *.xml test_results'
                junit '**/bin/test_results/*.xml'
            }
        }, "windows unit tests":{
            node ("windows") {
                bat 'cd bin/Release && test_core.exe -o junit && mkdir test_results && move *.xml test_results'
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
