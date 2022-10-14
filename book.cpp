#include "book.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <limits>
//
// Constructors, Assignments, and Destructor
//

Book::Book(const std::string& title,
           const std::string& author,
           const std::string& isbn, 
           double price) : title_(title), author_(author), isbn_(isbn), price_(price) {}

Book::Book(const Book& other) = default;

Book& Book::operator=(const Book& rhs) = default;

// Destructor
Book::~Book() noexcept = default;

//
//  Accessors
//

const std::string& Book::isbn() const {
  // returns a const reference to Book's isbn
  return isbn_;
}

const std::string& Book::title() const {
  // returns a const reference to Book's title
  return title_;
}

const std::string& Book::author() const {
  // returns a const reference to Book's author
  return author_;
}

double Book::price() const {
  //returns the price of Book
  return price_;
}

std::string Book::isbn() {
  // returns the isbn of a Book object 
  return isbn_;
}

std::string Book::title() {
  // returns the title of a Book object
  return title_;
}

std::string Book::author() {
  // returns the author of a Book object
  return author_;
}

//
// Modifiers
//

Book& Book::isbn(const std::string& new_isbn) {
  // sets the isbn of this object to new_isbn
  isbn_ = new_isbn;
  // returns the current instance of the Book class
  return *this;
}

Book& Book::title(const std::string& new_title) {
  // sets the title of this object to new_title
  title_ = new_title;
  // returns the current instance of Book class
  return *this;
}

Book& Book::author(const std::string& new_author) {
  // sets the author of this object to new_author
  author_ = new_author;
  // returns the current instance of Book
  return *this;
}

Book& Book::price(double new_price) {
  // sets the price of this object to new_price
  price_ = new_price;
  // returns the current instance of Book
  return *this;
}

//
// Relational Operators
//

bool Book::operator==(const Book& rhs) const noexcept {
  // All attributes must be equal for the two books to be equal to the other. 
  //
  // This can be done in any order, so put the quickest and the most likely
  // to be different first.

  return isbn_ == rhs.isbn_ && title_ == rhs.title_ && author_ == rhs.author_ && price_ == rhs.price_;
}

bool Book::operator!=(const Book& rhs) const noexcept {
  // Two books are unequal if any of their attributes are unequal.

  // uses the equality operator to check if two Book objects are not equivalent 
  return !(*this == rhs);
}

bool Book::operator<(const Book& rhs) const noexcept {
  // Books are ordered (sorted) by ISBN, author, title, then price.

  // if isbn_ is not equal to rhs.isbn_, then return if isbn_ is less than rhs.isbn_
  if (isbn_ != rhs.isbn_) {
    return isbn_ < rhs.isbn_;
  }
  // returns if author_ is less than rhs.author_, if they are not equal
  if (author_ != rhs.author_) {
    return author_ < rhs.author_;
  }
  // returns if title_ is less than rhs.title_, if they are not equal
  if (title_ != rhs.title_) {
    return title_ < rhs.title_;
  }

  // returns if price_ is less than rhs.price_, if they are not equal
  if (price_ != rhs.price_) {
    return price_ < rhs.price_;
  }
  // return false if Book is equal to rhs. 
  return false;
}

bool Book::operator<=(const Book& rhs) const noexcept {
  // Books are ordered (sorted) by ISBN, author, title, then price.

  // returns if Book is less than or equal to rhs by using less than and equality operators 
  return *this < rhs || *this == rhs;
}

bool Book::operator>(const Book& rhs) const noexcept {
  // Books are ordered (sorted) by ISBN, author, title, then price.

  // uses the < operator to check if rhs is less than the current instance of Book.
  //If rhs is less than *this, then *this > rhs. 

  return rhs < *this;
}

bool Book::operator>=(const Book& rhs) const noexcept {
  // Books are ordered (sorted) by ISBN, author, title, then price.

    // uses the <= operator to check if rhs is less than or equal to the current instance of Book. 
    // If rhs is less than or equal to *this, then *this >= rhs. 
    return rhs <= *this;
}

//
// Insertion and Extraction Operators


std::istream & operator>>(std::istream& stream, Book& book) {
  // A lot can go wrong when reading from streams - no permission, wrong types,
  // end of file, etc. Minimal exception guarantee says there should be no side
  // affects if an error or exception occurs, so let's do our work in a local object
  // and move it into place at the end if all goes well.
  //
  // This function should be symmetrical with operator<< below.
  //
  // Assume fields are separated by commas and string attributes are enclosed
  // with double quotes.  For example:
  //    ISBN             | Title                 | Author             | Price
  //    -----------------+-----------------------+--------------------+-----
  //    "9789998287532",   "Over in the Meadow",   "Ezra Jack Keats",   91.11
  //
  //
  // Hint:  Use std::quoted to read and write quoted strings.  See
  //        1) https://en.cppreference.com/w/cpp/io/manip/quoted
  //        2) https://www.youtube.com/watch?v=Mu-GUZuU31A

  // Reads the istream data into a temporary Book object.
  // This makes sure that book is not mutated if stream 
  // throws an exception 
  
  Book temp_book;
  stream >> std::quoted(temp_book.isbn_);
  // use std::numeric_limits<std::streamsize>::max() to ignore as many 
  // characters as needed until the delim ',' is found 
  stream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
  stream >> std::quoted(temp_book.title_);
  stream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
  stream >> std::quoted(temp_book.author_);
  stream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
  stream >> temp_book.price_;

  // The temporary book object is moved into book since 
  // the stream was read from successfully 
  book = std::move(temp_book);
  
  return stream; 
}

std::ostream& operator<<(std::ostream& stream, const Book& book) {
 
  // This function should be symmetrical with operator>> above.
  // Insert isbn (quoted), a comma, title (quoted), a comma, 
  // author (quoted), a comma, and price into stream. 
  stream << std::quoted(book.isbn_)
      << ","
      << std::quoted(book.title_)
      << ","
      << std::quoted(book.author_) 
      << ","
      << book.price_
      << std::endl;
  return stream;
}