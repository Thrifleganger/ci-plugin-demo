/*
	This is a declarative pipeline script for Jenkins for Projucer type JUCE plugin projects
	Fill the variables at the start of this file to configure the build pipeline for your plugin.
	The build pipeline performs the following stages: 
	1) Checkout: The build pipeline is triggered for every change to the master branch. The Checkout stage clones the project locally within the Jenkins executor.
	2) Build: Depending on the OS, the corresponding build systems is utilized to build VST3 and AU targets.
	3) Test: The plugin is subjected to a testing application called pluginval. 
	4) Deploy: The build artifacts, including the VST3 and AU files are uploaded to an S3 bucket.
	5) Notify: A slack notification is sent out to the corresponding channel to notify whether a build has succeeded or failed.
*/

/* 
	The project name is the name of projucer file / plugin name configured within the projucer file. Avoid spaces.
*/
def projectName = 'ci-plugin-demo'

/*
	The clone URL of the git repo. Add the SSH variant which starts with "git@github.com"
*/
def gitRepoUrl = 'git@github.com:Thrifleganger/ci-plugin-demo.git'

def version = ''

/*
	Start of pipeline script. 
*/
pipeline {
    agent any
	triggers {
		pollSCM('*/2 * * * *')
	}	
    stages {
		stage('Checkout') {
			steps {	
				cleanWs()
				checkout([
					$class: 'GitSCM', 
					branches: [[name: '*/master']], 
					extensions: [], 
					userRemoteConfigs: [[
						credentialsId: 'jenkins-to-github', 
						url: "${gitRepoUrl}"
					]]
				])
			}
		}
        stage('Build') {   
        	steps {     
        		echo "Build"   		
        	}          
        }
        stage('Test') {             
        	steps {    
        		echo "Test"    		
        	}
        }
		stage('Package') {			
			steps {		
				echo "Package"		
			}
		}		
    }
}   
