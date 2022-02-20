#include <iostream>
#include <climits>

#include "pancake_stack.cc"

int main() {
	using namespace std;
	using namespace daily_programmer_20180307;
	
	string str_number_of_pancakes;
	int number_of_pancakes;

	getline(cin, str_number_of_pancakes);
	try {
		number_of_pancakes = stoi(str_number_of_pancakes);
	} catch (invalid_argument) {
		cout << "Input must be a positive integer" << endl;
		return 1;
	} catch (out_of_range) {
		cout << "Too many pancakes" << endl;
		return 1;
	}

	auto pancakes = vector<int>(number_of_pancakes);
	for (int i = 0; i < number_of_pancakes; i++) {
		string str_pancake;
		cin >> str_pancake;
		int pancake;
		try {
			pancake = stoi(str_pancake);
		} catch (invalid_argument) {
			cout << "Each pancake must be an integer" << endl;
			return 1;
		} catch (out_of_range) {
			cout << "Pancakes larger than " << INT_MAX << " are not supported" << endl;
			return 1;
		}
		pancakes[i] = pancake;
	}

	auto pancake_stack = PancakeStack(pancakes);
	pancake_stack.Sort();

	cout << pancake_stack.get_number_of_flips() << endl;

	return 0;
}