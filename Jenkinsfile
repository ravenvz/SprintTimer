node {
    stage("Checking out source code") {
        node ("linux") {
            deleteDir();
            git credentialsId: 'BitbucketRepoCred', poll: false, url: 'https://bitbucket.org/Vizier/pomodoro'
        }
        node ("windows") {
            deleteDir();
            git credentialsId: 'BitbucketRepoCred', poll: false, url: 'https://bitbucket.org/Vizier/pomodoro'
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
        node ("linux") {
            sh "cd build && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=On .. && make -j\$(nproc)"
        }
        node ("windows") {
            // TODO clean this up
            bat 'cd build && cmake .. -G "Visual Studio 14 Win64" -DCMAKE_PREFIX_PATH="C:/Qt/5.7/msvc2015_64" -DCMAKE_INCLUDE_PATH="C:/Users/Pavel/Downloads/boost_1_62_0/boost_1_62_0" -DBUILD_TESTS=ON -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE -DBUILD_SHARED_LIBS=TRUE && cmake --build . --config Release'
        }
    }
    stage('Run tests') {
        node ("linux") {
            sh 'cd bin && ./acceptance_tests_stub -o junit && ./test_core -o junit && ./test_qt_storage_impl -o junit'
            sh 'mkdir bin/test_results && mv bin/*.xml bin/test_results'
            junit '**/bin/test_results/*.xml'
        }
        node ("windows") {
            env.PATH = "C:/Qt/5.7/msvc2015_64:${env.PATH}"
            bat 'cd bin && acceptance_tests_stub -o junit && test_core.exe -o junit && test_qt_storage_impl -o junit'
            bat 'mkdir bin/test_results && move *.xml test_results'
            junit '**/bin/test_results/*.xml'
        }
    }
    stage('Archive binaries') {
        node ("linux") {
            archiveArtifacts '**/lib/*.so'
            archiveArtifacts '**/bin/*'
        }
        node ("windows") {
            archiveArtifacts '**/bin/*'
        }
    }
}

