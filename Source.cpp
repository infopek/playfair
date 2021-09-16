#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

void correct(std::string& s)
{
	auto lowercase = [](char c) {return std::tolower(c); };
	auto toalphchar = [](char c) {return (char)(97 + abs((int)c % 26)); };

	// Remove whitespaces
	s.erase(std::remove(s.begin(), s.end(), ' '), s.end());

	// Max length: 25
	s = s.substr(0, 25);

	// Lowercase only
	std::transform(s.begin(), s.end(), s.begin(), lowercase);

	// Only characters in the lowercase alphabet
	std::transform(s.begin(), s.end(), s.begin(), toalphchar);
}

void removeDups(std::vector<char>& v)
{
	// Removes all duplicates of a given vector of chars, keeps order
	std::unordered_set<char> seen;

	auto insertion = [&seen](char c)
	{
		return !seen.insert(c).second;
	};

	v.erase(std::remove_if(v.begin(), v.end(), insertion), v.end());
}

void generateMatrix(std::vector<std::vector<char>>& m, const std::string& kw)
{
	m.resize(5, std::vector<char>(5));

	// The English alphabet (25 letters, letter j is removed by default)
	std::vector<char> alphabet = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k', 'l',
	'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

	std::vector<char> kwChars;
	for (auto& c : kw)
	{
		kwChars.push_back(c);
	}

	removeDups(kwChars);

	// Form the union of the two vectors
	std::vector<char> vUnion;

	for (auto& c : kwChars)
	{
		alphabet.erase(std::remove(alphabet.begin(), alphabet.end(), c), alphabet.end());
		vUnion.push_back(c);
	}

	for (auto& c : alphabet)
	{
		vUnion.push_back(c);
	}

	// Fill m
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			m[i][j] = vUnion[5 * i + j];
		}
	}

}

void chunkify(std::string& s, std::vector<std::vector<char>>& v)
{
	// Rules
		// 1. Insert x wherever a duplicate pair is found (pair with same letters)
		// 2. Insert x if last pair has only 1 element

	for (int i = 0; i < s.size() - 1; i++)
	{
		if (s[i] == s[i + 1])
		{
			s.insert(i + 1, 1, 'x');
		}

		if (s[i] == 'j')
		{
			s[i] = 'i';
		}
	}

	if (s.size() % 2 != 0)
	{
		s.push_back('x');
	}

	// Resize vector now
	v.resize(s.size() / 2, std::vector<char>(2));

	for (int i = 0; i < s.size() / 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			v[i][j] = s[2 * i + j];
		}
	}
}

void encipher(const std::vector<std::vector<char>>& mat, std::vector<std::vector<char>>& msg)
{
	for (int i = 0; i < msg.size(); i++)
	{
		char* first = &msg[i][0];
		char* second = &msg[i][1];

		// Finding indices (c1_x, c1_y), (c2_x, c2_y)
		int first_row = -1, first_col = -1, second_row = -1, second_col = -1;
		for (int m = 0; m < 5; m++)
		{
			for (int n = 0; n < 5; n++)
			{
				if (mat[m][n] == *first)
				{
					first_row = m;
					first_col = n;
				}
				else if (mat[m][n] == *second)
				{
					second_row = m;
					second_col = n;
				}
			}
		}

		if (first_row == second_row) // Same row, shift right (wrap around if needed)
		{
			*first = mat[first_row][(first_col + 1) % 5];
			*second = mat[second_row][(second_col + 1) % 5];
		}
		else if (first_col == second_col) // Same col, shift down (wrap around if needed)
		{
			*first = mat[(first_row + 1) % 5][first_col];
			*second = mat[(second_row + 1) % 5][second_col];
		}
		else // Create box and swap letter on the end of rectangle
		{
			*first = mat[first_row][second_col];
			*second = mat[second_row][first_col];
		}
	}
}

int main()
{
	std::string keyword;
	std::string msg;
	std::string cipher = "";

	std::vector<std::vector<char>> mat5x5;
	std::vector<std::vector<char>> msgChunks;

	// Get input keyword
	std::cout << "Keyword: ";
	std::cin.ignore(0, '\n');
	std::getline(std::cin, keyword);

	// Get input message
	std::cout << "Message: ";
	std::cin.ignore(0, '\n');
	std::getline(std::cin, msg);

	correct(keyword);
	correct(msg);

	generateMatrix(mat5x5, keyword);
	chunkify(msg, msgChunks);
	encipher(mat5x5, msgChunks);

	for (int i = 0; i < msgChunks.size(); i++)
	{
		for (int j = 0; j < 2; j++)
		{
			cipher += msgChunks[i][j];
		}
	}

	std::cout << "Encoded message: " << cipher << std::endl;

	return 0;
}