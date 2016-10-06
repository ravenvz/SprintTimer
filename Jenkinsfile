node ("archlinux") {
    stage("Checking out source code") {
        deleteDir();
        git credentialsId: '695c1adc-c31e-4f30-b05d-53ba3a6e8a16', poll: false, url: 'https://bitbucket.org/Vizier/pomodoro'
    }
    stage('Run static analisys') {
        sh "cppcheck --xml --xml-version=2 . 2> check.xml"
    }
    stage('Build') {
        sh "cd build && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=On .. && make -j\$(nproc)"
    }
    stage('Run tests') {
        sh 'cd bind && ./acceptance_tests_stub -o junit && ./test_core -o junit && ./test_qt_storage_impl -o junit'
        sh 'mkdir bin/test_results && mv bin/*.xml bin/test_results'
        junit '**/bin/test_results/*.xml'
    }
    stage('Archive binaries') {
        archiveArtifacts '**/lib/*.so'
        archiveArtifacts '**/bin/*'
    }
}
