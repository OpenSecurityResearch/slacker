#include "slackRemover.h"



int main(int argc, char** argv)
{
	//Call class
	slackInfo slack;
	
	//Boost set up menu and options
	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()
	("fileclean,f", po::value<string>(), "Choose a file to clean its slack space")
	("driveclean,d", po::value<string>(), "Choose a drive to clean all of its files ")
	("help,h", "Displays help message with options");


	//Boost parse command line 
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);


	//if statements will navigate the user through the options for their desired action
	if (vm.count("help")){
		cout << "File Slack Wiper\n\nHow to use: fileSlack [-fd ARG] [-th]\n\n"<<desc << "\n";
	}
	
	else if (vm.count("fileclean")){
		cout << "You have entered file: " << vm["fileclean"].as<string>() << endl;
		slack.ClearSlackdum(slack.fileCreatedum(vm["fileclean"].as<string>()));
			
	}

	else if (vm.count("driveclean")){
		//seems to be a string buffer overload if the directory is too long
		//On a second note, it looks like it doesn't like spaces(maybe numbers)
		cout << "Target Directory: " << vm["driveclean"].as<string>() << "\n\n\n";
		slack.allDirFilesdum(vm["driveclean"].as<string>());
	}
	
	else{
		cout << "You have entered an invalid input. Please refer back to the options below:\n\n" << desc << endl;
	}
	
	system("pause");

	return 0;
}