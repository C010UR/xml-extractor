#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::fstream;
using std::getline;

/**
 * @brief Trims a string and adds a space to the end.
 * 
 * @param str input string
 * @return string trimmed string
 */
string trim(const string &str) {
  string::size_type left = 0, right = 0;
  string::size_type str_length = str.length();
  for (string::size_type i = 0; i < str_length; i++) {
    if (!isspace(str[i])) {
      left = i;
      break;
    }
  }

  for (string::size_type i = str_length - 1; i > 0; i--) {
    if (!isspace(str[i])) {
      right = i;
      break;
    }
  }

  string res = str.substr(left, right - left + 1);
  return res.length() == 0 ? "" : res + " ";
}

/**
 * @brief Gets index of specified symbol if it is present in the string.
 * 
 * @param str input string
 * @param str_length string length (if not specified will be calculated based on the input string)
 * @param symbol symbol to search
 * @param start starting position of the search (defaults to 0)
 * @return string::size_type 0 if string is empty or index of the character if present
 */
string::size_type cursor_position(
                                  const string &str, 
                                  string::size_type str_length, 
                                  const char symbol, 
                                  const string::size_type start = 0
                                  ) {
  if (str_length == 0) str_length = str.length();

  for (string::size_type i = start; i < str_length; i++) {
    if (str[i] == symbol) return i;
  }
  return str_length == 0 ? 0 : str_length - 1;
}

/**
 * @brief Extracts part of a string up to specified symbol.
 * 
 * @param str input stream
 * @param str_length string length (if not specified will be calculated based on the input string)
 * @param symbol symbol to search
 * @param start starting position of the search (defaults to 0)
 * @return string extracted string
 */
string extract (
                const string &str,
                string::size_type str_length,
                const char symbol,
                const string::size_type start = 0
                ) {
  if (str_length == 0) str_length = str.length();
  
  string buff = ""; 
  for (string::size_type i = start; i < str_length; i++) {
    if (str[i] == symbol) {
      return trim(buff);
    }
    buff += str[i];
  }

  return trim(buff);
}

/**
 * @brief Removes tags from basic XML string
 * 
 * @param str input string
 * @return string result
 */
string convert(const string &str) {
  string result = "";
  string temp = "";
  string::size_type str_length = str.length();
  
  string::size_type left_cursor = 0;
  
  // get start of str, if it is not xml
  string::size_type start = cursor_position(str, str_length, '<');
  if (start == str_length - 1) return str;
  if (start > 0 && start < cursor_position(str, str_length, '>'))
    result += trim(str.substr(0, start));

  // main loop
  while ((left_cursor = cursor_position(str, str_length, '>', left_cursor) + 1) < str_length - 1) {
    result += extract(str, str_length, '<', left_cursor);
  }
  return result;
}

int main(int argc, char** argv) {

  if (argc < 2) {
    cout << "No input file specified" << endl;
    return EXIT_FAILURE;
  }

  string filename (argv[1]);

  fstream input (filename, fstream::in); 
  if (!input.is_open()) return EXIT_FAILURE;

  fstream output ("result.txt", fstream::out);
  if (!input.is_open()) return EXIT_FAILURE;

  string str;
  while (input.good()) {
    getline(input, str);
    if ((str = trim(str)) != "") {
      output << convert(str);
    }
  }
  
  return EXIT_SUCCESS;
}