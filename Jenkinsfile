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
            stage('Run static analysis') {
                sh "cppcheck --xml --xml-version=2 . 2> check.xml"
            }
        }
        node ("windows") {

        }
    }
    stage('Build') {
        node ("linux") {
            sh "cd build && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=On .. && make -j\$(nproc)"
        }
        node ("windows") {

        }
    }
    stage('Run tests') {
        node ("linux") {
            sh 'cd bin && ./acceptance_tests_stub -o junit && ./test_core -o junit && ./test_qt_storage_impl -o junit'
            sh 'mkdir bin/test_results && mv bin/*.xml bin/test_results'
            junit '**/bin/test_results/*.xml'
        }
        node ("windows") {

        }
    }
    stage('Archive binaries') {
        node ("linux") {
            archiveArtifacts '**/lib/*.so'
            archiveArtifacts '**/bin/*'
        }
        node ("windows") {

        }
    }
}

