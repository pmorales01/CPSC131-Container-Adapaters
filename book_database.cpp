
// Include necessary header files.
//
// Do not put anything else in this section, i.e. comments, classes,
// functions, etc. Only #include directives.

#include "book_database.hpp"

#include <fstream>
#include <string>

#include "book.hpp"


const std::string kDatabaseFilename = "database.txt";

// Return a reference to the one and only instance of the database
BookDatabase& BookDatabase::instance() {
  static BookDatabase theInstance(kDatabaseFilename);
  return theInstance;
}

BookDatabase::BookDatabase(const std::string& filename) {
  std::ifstream fin(filename, std::ios::binary);

  // The file contains Books separated by whitespace. A Book has 4 pieces of
  // data delimited with a comma. This exactly matches the previous assignment
  // as to how Books are read.
  //
  //       Field            Type            Notes
  //  1.   Book's ISBN      String          Unique identifier (primary key), always enclosed in double quotes
  //  2.   Book's Title     String          May contain spaces, always enclosed in double quotes
  //  3.   Book's Author    String          May contain spaces, always enclosed in double quotes
  //  4.   Book's Price     Floating Point  In dollars
  //
  //  Example:
  //    "0001062417",  "Early aircraft",                 "Maurice F. Allward", 65.65
  //    "0000255406",  "Shadow maker \"1st edition)\"",  "Rosemary Sullivan",   8.08
  //    "0000385264",  "Der Karawanenkardinal",          "Heinz Gstrein",      35.18
  //
  // NOTE: Double quotes within the string are escaped with the backslash
  // character.
  //
  
  
  // HINT:  Use your Book's extraction operator to read Books, don't reinvent
  // that here. Read books until end of file, pushing each book into the data
  // store as they are read.
  
  // Create a Book object to use for reading.
  Book temp;

  // Use a while loop to read the Books from the file 
  // and push them to the database. 
  while (fin >> temp) {
    // push temp to the data store
    database_.push_back(temp);
  }

  // NOTE: The file is intentionally not explicitly closed. The file is closed
  // when fin goes out of scope - for whatever reason. More precisely, the
  // object `fin` is destroyed when it goes out of scope and the file is closed
  // in the destructor. See RAII.
}


// Implement the rest of the interface, including functions find
// (recursively) and size.
//
// Programming note:  An O(n) operation, like searching an unsorted vector,
// would not generally be implemented recursively. The depth of recursion
// may be greater than the program's function call stack size. But for this
// programming exercise, getting familiar with recursion is a goal.

// Helper Function for BookDatabase::find()
Book* BookDatabase::find_helper_function(const std::string& isbn, int index) {
  // Check to see if index is negative or greater than 
  // or equal to the memory resident data store's size.
  if (index < 0 || index >= database_.size()) {
    // Return nullptr if index is not valid, or 
    // if the book has not been found. 
    return nullptr;
  } 

  // Create a temporary std::string to store the isbn 
  // of the Book at the current index position. 
  std::string isbn_at_index = database_[index].isbn();

  // If isbn is equal to the isbn of the current Book.
  if (isbn == isbn_at_index) {
    // Create a pointer to the Book at the current 
    // index position of database_. 
    Book* book_ptr = &database_[index];
    
    // Return the pointer, the book has been found. 
    return book_ptr;
  } else { /* The book has not been found. */
    // Increment the index. 
    index++;
    
    // Search for the next book in database_
    return find_helper_function(isbn, index);
  }

}

// Wrapper Function is BookDatabase::find()
Book* BookDatabase::find(const std::string& isbn) {
  return find_helper_function(isbn, 0);
}

std::size_t BookDatabase::size() const {
  // Use std::vector::size() to return size of database
  return database_.size();
}
