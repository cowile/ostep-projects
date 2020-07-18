/*
 * =====================================================================================
 *
 *       Filename:  reverse.cc
 *
 *    Description:  Reverses the lines of a file
 *
 *        Version:  1.0
 *        Created:  07/04/2020 14:49:10
 *       Revision:  none
 *       Compiler:  clang++
 *
 *         Author:  Brian Li,
 *   Organization:
 *
 * =====================================================================================
 */
#include <iostream>
#include <stdexcept>
#include <sys/stat.h>

template <class T> class Vector {
private:
  // Backing array
  T *arr_;
  int cap_;
  int len_;

public:
  // Constructor
  Vector() {
    cap_ = 1;
    len_ = 0;

    // Initialize a to point to an array of length `cap`
    arr_ = new T[cap_ * sizeof(T)];
  }

  // Inserts `v` at the end
  void insert(const T &v) {
    if (len_ == cap_) {
      // Copy `arr` into a new array
      cap_ *= 2;
      T *nextArr = new T[cap_ * sizeof(T)];
      for (int i = 0; i < len_; i++) {
        nextArr[i] = arr_[i];
      }
      delete[] arr_;
      arr_ = nextArr;
    }

    arr_[len_] = v;
    len_ += 1;
  }

  // Gets the last element
  T get() { return arr_[len_ - 1]; }

  // Gets the element at index `i`
  T get(const int i) {
    if (i < 0 || i >= len_) {
      char errMsg[100];
      sprintf(errMsg, "Expected an index between 0 and %d, got %d", len_ - 1,
              i);
      throw std::out_of_range(errMsg);
    }
    return arr_[i];
  }

  // Returns the length of the array
  const int getSize() { return len_; }
};

int main(int argc, char *argv[]) {
  if (argc > 3) {
    std::cerr << "usage: reverse <input> <output>\n";
    return 1;
  }

  FILE *in = stdin;
  if (argc > 1) {
    char *input = argv[1];
    in = fopen(input, "r");
    // Verify that `input` exists
    if (in == NULL) {
      std::cerr << "reverse: cannot open file '" << input << "'\n";
      return 1;
    }
  }

  FILE *out = stdout;
  if (argc == 3) {
    char *input = argv[1];
    char *output = argv[2];

    // Verify that the `input` is physically different from `output`
    struct stat inStat, outStat;
    stat(input, &inStat);
    stat(output, &outStat);
    if (inStat.st_ino == outStat.st_ino) {
      std::cerr << "reverse: input and output file must differ\n";
      return 1;
    }

    out = fopen(output, "w");
    // Verify that `output` exists
    if (out == NULL) {
      std::cerr << "reverse: cannot open file '" << output << "'\n";
      return 1;
    }
  }

  Vector<std::string> v;

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, in)) > 0) {
    v.insert(line);
  }

  for (int i = v.getSize() - 1; i >= 0; i--) {
    fprintf(out, "%s", v.get(i).c_str());
  }

  fclose(in);
  fclose(out);
  return 0;
}
