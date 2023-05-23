/*
 * reasoner.h
 *
 *  Created on: 6 Oct 2020
 *      Author: Home
 */
#include <string>
#include <vector>
#ifndef REASONER_H_
#define REASONER_H_

using namespace std;

class predicate {
public:
	string name;
	string farg;
	string sarg;

	string endInput = ("nil");
	string startUp = "Now please input your knowledge base!\n"
			"When you finish your input, just type ";

	vector<string> kb;
	vector<string> LS, RS;
	vector<string> LS_B;
	vector<string> visited;
	vector<string> branch;
	vector<string> variables;
	vector<string> proven;

	char proveChar;

	bool isOR = false;
	bool isAND = false;

	bool conjunct = false;
	bool disjunct = false;

	char disjunction = '^';
	char conjunction = 'v';

	bool isAlpha(char chr) {
		if (((chr <= 'z' && chr >= 'a') || (chr <= 'Z' && chr >= 'A'))) {
			return true;
		}
		return false;
	}


	void storeWords() {
		string input;
		string line;
		cout << startUp << endInput << endl;
		do {
			//getline(cin,input);
			cin >> input;
			cout << "Input:" << input << endl;
			cin.clear();
			if (input != endInput) {
				/*don't add terminating key to KB*/
				kb.push_back(input);
				if (input.size() == 1) {
					//cout << "Variable: " << input << endl;
					variables.push_back(input);
				}
			}
		} while (input.compare(endInput) != 0);
		if (input.compare(endInput) == 0) {
			cout << input << " == " << endInput << endl;
			cout
					<< "You have finished your input. Now you can test your system!"
					<< endl;
			for (unsigned i = 0; i < kb.size(); i++) {
				cout << kb[i] << " ";
			}
			cout << endl;
		}
	}

	void verifyQuery(char atom) {
		//build goal states
		int count = -1;
		for (unsigned i = 0; i < kb.size(); i++) {
			for (unsigned it = 0; it < kb[i].length(); it++) {
				//cout << kb[i].at(it);
				if (atom == kb[i].at(it)) {
					count++;
					if (count % 2 == 0 && it == kb[i].length()-1) {
						cout << "Inserting " << kb[i] << endl;
						LS.push_back(kb[i]);
						break;
					}
					else if(count % 2 != 0 && it == kb[i].length()-1){
						cout << "RS: Inserting " << kb[i] << endl;
						RS.push_back(kb[i]);
						// fix LS.push_back(kb[i]);
						break;
					}
				}
			}
		}
	}

	bool hasVisited(string node) {
		for (unsigned it = 0; it < visited.size(); it++) {
			if(node == visited[it]) {
				return true;
			}
		}
		return false;
	}
	bool isDone(vector<string> &vec) {
		if(vec.size() > 1) {
			return false;
		}
		return true;
	}

	bool inVariables(char atom) {
		for (unsigned i = 0; i < variables.size(); i++) {
			if(atom == variables[i].at(0)) {
				//cout << "Found!" << endl;
				return true;
			}
		}
		return false;
	}

	int extendedVerifyQuery(char atom, vector<string> &vec) {
		int count = -1;
		vector<string> temp;
		cout << "Running " << endl;
		for (unsigned i = 0; i < kb.size(); i++) {
			if(inVariables(atom)) {
				count = 0;
				break;
			}
			for (unsigned it = 0; it < kb[i].length(); it++) {
				//cout << kb[i].at(it);
				if (atom == kb[i].at(it)) {
					cout << "Inserting " << kb[i] << endl;
					vec.push_back(kb[i]);
					count++;
					break;
				}
			}
		}

		return count;
	}
	int impliedInKB(char atom, vector<string> &vec) {
		int count = -1;
		//cout << count << endl;
		cout << "Searching for " << atom << endl;
		for (unsigned i = 0; i < kb.size(); i++) {
			if(inVariables(atom)) {
				count = 0;
				break;
			}
			for (unsigned it = 0; it < kb[i].length(); it++) {
				//cout << kb[i].at(it);
				if (atom == kb[i].at(kb[i].length()-1)) {
					cout << "Inserting " << kb[i] << endl;
					vec.push_back(kb[i]);
					count++;
					break;
				}
			}
		}
		//cout << count << endl;
		return count;
	}
	void extractCS(vector<string> vect, vector<string> &bQueue) {
		string sentence;
		//cout << "Extraction" << endl;
		sentence = vect[0];
		for(char& c : sentence) {
			//cout << c << endl;
			if(c == conjunction) {
				conjunct = true;
				//cout << "Setting conjunction." << endl;
			}
			if (c == disjunction) {
				disjunct = true;
				//cout << "Setting Disjunction." << endl;
			}
			if (c == '=' || c == '>') {
				break;
			}
			else {
				if(isAlpha(c) && c != 'v') {
					string s;
					s.push_back(c);
					//cout << "pushing in " << s <<endl;
					bQueue.push_back(s);
				}
			}
		}
	}

	void expandTree(vector<string> tree) {
		int step = 1;
		bool done = false;
		//dfs
		int num = 0;
		int status = -1;
		string current;
		current = tree.back();
		visited.push_back(current);
//		char precedence;
		//used to store the extended branch
		vector<char> sentence;
		vector<string> queue;
		vector<string> bQueue;
//		sentence.push_back(current);
		analyzeCS(current);
		cout << "BS: " << branch.size() << endl;
		char atom;
		atom = branch.front().at(0);
		cout << "Pre-check Atom " << atom << endl;
		status = impliedInKB(atom, queue);
		if(status == 0) {
			branch.erase(branch.begin());
		}

		do {

			atom = branch.front().at(0);
			cout << "Atom " << atom << endl;
			status = impliedInKB(atom, queue);
			testPrintVector(queue);

			if(step % 2 == 0) {
				isAND = true;
			}
			else {
				isOR = true;
			}
			if(isOR) {
				if(queue.size() < 1) {
					done = true;
					break;
				}
				int s;
				//cout << "OR size: " << endl;
				cout << queue[0] << endl;
				extractCS(queue, bQueue);
				s = bQueue.size();;
				int c = 0;
				for(unsigned i = 0; i <= bQueue.size(); i++) {
					cout << bQueue[1] << endl;
					char newQuery = bQueue.front().at(0);
					cout << "Loading new Query: " << newQuery << " for ";
					cout<< queue[0] << endl;
//					cout << status << endl;
//					cout << "Size: " << bQueue.size() << endl;
//					testPrintVector(bQueue);
//					bQueue.erase(bQueue.begin());
//					cout << "Size: " << bQueue.size() << endl;
//					testPrintVector(bQueue);
					//should return either 0 or
					//				cout << newQuery << endl;
					status = impliedInKB(newQuery,queue);
					//testPrintVector(bQueue);
					cout << "Status: " << status << endl;
					if (status == -1 && disjunct) {
						break;
					}
					if (status == -1 && conjunct) {
						cout << "Moving onto next Atom " << endl;
						bQueue.erase(bQueue.begin());
						break;
					}
					if (status == 0){
						//cout << bQueue.front().length();
						bQueue.erase(bQueue.begin());
					}
//					cout << "It..: " << i << endl;
					c++;

				}
				if (status == 0 && disjunct) {
					done = true;
					break;
				}
				if(status == 0 && queue.size() > 1) {
					queue.erase(queue.begin());
					testPrintVector(queue);
				}
				if(status == 0 && queue.size() == 1) {
					branch.erase(branch.begin());
				}
				if (c == s && status == 0) {
					//cout << "Erasing queue +1 " << endl;
					queue.erase(queue.begin());
					testPrintVector(queue);
				}
				if(queue.size() < 1) {
					done = true;
					break;
				}
				//cout << "Status: " << status << ",Size: " << queue.size() << endl;
				num++;
				//cout << num << endl;
				queue.erase(queue.begin());
			}
			if(isAND) {
				cout << "AND" << endl;
				char newQuery = queue.front().at(0);
				cout << "Loading new Query: " << newQuery << endl;
				//cout << status << endl;
				queue.erase(queue.begin());
				testPrintVector(queue);
				//should return either 0 or
				cout << newQuery << endl;
				status = extendedVerifyQuery(newQuery,queue);
				//cout << "Status: " << status << endl;
				num++;
				//cout << num << endl;
			}

		}
		while(!done || num > 2);
		if(done) {
			cout << "Query: " << proveChar << " is true" << endl;
			queue.clear();
			bQueue.clear();
			branch.clear();
			LS.clear();
		}

		//cout << current;

	}

	void analyzeCS(string sentence) {
		char base;
		for (unsigned it = 0; it < sentence.length(); it++) {
			base = sentence.at(it);
			if(isAlpha(base) && base != proveChar) {
				cout << base << endl;
				string s;
				s.push_back(base);
				branch.push_back(s);
			}
		}
	}

	void testKB() {
		string query;
		char atom;
		string end = "quit";
		/*Breakdown if kb[i].length() > 1 (It is a predicate)
		 * if kb[i].length == a (it is a variable)
		 * search the kb for matching values eg if 'F' it would return:
		 *  D^E=> F and  B^E=>F --> goal vector
		 *  create a left and right tree
		 */
		do {
			cout << "Enter a query: " << endl;
			cin >> query;
			if (query.length() < 2) {
				atom = query.at(0);
				prove(atom);

			}
		}while (query.compare(end) != 0);
	}

	void prove(char query) {
		proveChar = query;
		verifyQuery(query);
		if(inVariables(query)){
			cout << "True" << endl;
		}
		if(LS.size() > 0 && !inVariables(query)) {
			expandTree(LS);
		}
		if(!inVariables(query) && LS.size() < 1) {
			cout << "False" << endl;
		}

	}

	void testPrintVector(vector<string> v) {
		for (unsigned i = 0; i < v.size(); i++) {
			cout << v[i] << endl;
		}
	}

};

#endif /* REASONER_H_ */
