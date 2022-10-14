#include "checkout.hpp"

#include <iomanip>
#include <map>
#include <queue>
#include <stack>

#include "book.hpp"

namespace {

constexpr bool kOutputTrace = false;

// Call this function from within the carefully_move_books functions, just
// before kicking off the recursion and then just after each move.
void trace(
    std::stack<Book> const& sourceCart,
    std::stack<Book> const& destinationCart,
    std::stack<Book> const& spareCart,
    std::ostream& s = std::clog) {
  // Count and label the number of moves
  static std::size_t move_number = 0;

  // First time called will bind parameters to copies
  static std::map<std::stack<Book> const*, std::stack<Book>> book_carts = {
     {&sourceCart, {}},
     {&destinationCart, {}},
     {&spareCart, {}}
  };
  static std::map<std::stack<Book> const*, std::string> col_labels = {
      {&sourceCart, "Broken Cart"},
      {&destinationCart, "Working Cart"},
      {&spareCart, "Spare Cart"}
  };

  // Interrogating the stacks is a destructive process, so local copies of the
  // parameters are made to work with. The CarefullyMoveBooks algorithm will
  // swap the order of the arguments passed to this functions, but they will
  // always be the same objects - just in different orders.
  //
  // When outputting the stack contents, keep the original order so we humans
  // can trace the movements easier. A container (std::map) indexed by the
  // object's identity (address) is created so the canonical order remains the
  // same from one invocation to the next.
  book_carts[&sourceCart] = sourceCart;
  book_carts[&destinationCart] = destinationCart;
  book_carts[&spareCart] = spareCart;


  // Determine the size of the tallest stack.
  std::size_t tallest_stack_size = 0;
  for (auto& book_cart : book_carts) {
    if (book_cart.second.size() > tallest_stack_size) {
      tallest_stack_size = book_cart.second.size();
    }
  }

  if (kOutputTrace) {
    // Print the header
    s << "After " << std::setw( 3 ) << move_number++ << " moves:     " << std::left;      // print the move number
    for (const auto& col_label : col_labels) {
       s << std::setw( 23 ) << col_label.second;
    }
    s << std::right << "\n                     " << std::string( 23*3, '-' ) << '\n';     // underline the labels

    // Print the stack's contents.
    for (; tallest_stack_size > 0;  --tallest_stack_size) {
      s << std::string( 21, ' ' );

      for (auto& book_cart : book_carts) {
        if (book_cart.second.size() == tallest_stack_size) {
          std::string title = book_cart.second.top().title();
          if (title.size() > 20) {
            title[17] = title[18] = title[19] = '.';               // replace last few characters of long titles with "..."
          }
          s << std::left << std::setw( 23 ) << title.substr( 0, 20 ) << std::right;
          book_cart.second.pop();
        } else {
          s << std::string( 23, ' ' );
        }
      }
      s << '\n';
    }
    s << "                     " << std::string( 69, '=' ) << "\n\n\n\n";
  }
}

}  // namespace

Checkout::Checkout() : book_database_(BookDatabase::instance()) {}

/*
 * A recursive algorithm to carefully move books from a broken cart to a working
 * cart is given as follows:
 *   START
 *   Procedure carefully_move_books (
 *       number_of_books_to_be_moved, broken_cart, working_cart, spare_cart)
 *
 *    IF number_of_books_to_be_moved == 1, THEN
 *       move top book from broken_cart to working_cart
 *       trace the move
 *
 *    ELSE
 *       carefully_move_books (
 *           number_of_books_to_be_moved-1, broken_cart, spare_cart,
 *           working_cart)
 *       move top book from broken_cart to working_cart
 *       trace the move
 *       carefully_move_books (
 *           number_of_books_to_be_moved-1, spare_cart, working_cart,
 *           broken_cart)
 *
 *    END IF
 *
 * END Procedure
 * STOP
 *
 * As a side note, the efficiency class of this algorithm is exponential.
 * That is, the Big-O is O(2^n).
 */
void Checkout::carefully_move_books(
    std::size_t quantity, std::stack<Book>& broken_cart,
    std::stack<Book>& working_cart, std::stack<Book>& spare_cart) {
  
  // Implement the algorithm above.
  
  // If the quantity is 1.
  if (quantity == 1) {
    // Move the top book from broken_cart to  
    // working_cart by pushing it to working_cart. 
    working_cart.push(broken_cart.top());
    
    // Pop the top of broken_cart to represent removing a book from broken_cart. 
    broken_cart.pop();
    
    // Trace the move. 
    trace(broken_cart, working_cart, spare_cart);
  } else {  
    // Call carefully_move_books() to move the top book from broken_cart to spare_cart
    carefully_move_books(quantity - 1, broken_cart, spare_cart, working_cart);

    // Move the top book from broken_cart to  
    // working_cart by pushing it to working_cart. 
    working_cart.push(broken_cart.top());

    // Pop the top of broken_cart. 
    broken_cart.pop();

    // Trace the move. 
    trace(broken_cart, spare_cart, working_cart);

    // Call carefully_move_books() to move the top book from spare_cart to working_cart
    carefully_move_books(quantity - 1, spare_cart, working_cart, broken_cart);
  }
}

void Checkout::shop_for_books() {

  // Put the following books into your cart with the heaviest book on the
  // bottom and the lightest book on the top according to the ordering given
  // in the table below. The author should be left empty. The price should
  // not be specified (it will be retrieved from the database later).
  //
  //      ISBN             Title
  //      --------------   ----------------
  //      9780895656926    Like the Animals        <=== lightest book
  //      54782169785      131 Answer Key
  //      0140444300       Les Mis
  //      9780399576775    Eat pray love
  //      9780545310581    Hunger Games            <===  heaviest book

  // Since my_cart_ is a stack, it follows LIFO order.

  // Push the heaviest books to the stack first so that
  // they are at the bottom of the stack. 
  
  // Push the lightest books last to the stack so that
  // they can be at the top of the stack. 

  my_cart_.push(Book("Hunger Games", "", "9780545310581"));
  my_cart_.push(Book("Eat pray love", "", "9780399576775"));
  my_cart_.push(Book("Les Mis", "", "0140444300"));
  my_cart_.push(Book("131 Answer Key", "", "54782169785"));
  my_cart_.push(Book("Like the Animals", "", "9780895656926"));
}

void Checkout::switch_carts() {
  
  // Carefully move the books in your now broken cart to this working cart by
  // calling the carefully_move_books function.

  // Create a new stack of type Book to represent a spare cart.
  std::stack<Book> spare_cart;

  // Call carefully_move_books().    
  // 1st parameter takes in the size of my_cart_.
  // 2nd parameter takes in a broken cart which is my_cart_.
  // 3rd parameter takes in a working cart. 
  // 4th parameter takes in a spare cart. 
  carefully_move_books(my_cart_.size(), my_cart_, working_cart_, spare_cart);
}

void Checkout::move_books_to_checkout() {
  
  // Remove the books from your working cart and place them on the checkout
  // counter, i.e., put them in this checkoutCounter queue.
  
  // Create a const int to represent the size of your working cart before checkout. 
  const int original_size = working_cart_.size();

  for (int i = 0; i < original_size; ++i) {
    // Push the top book of working_cart_ to the checkoutCounter queue. 
    checkout_counter_.push(working_cart_.top());

    // Pop the top book of working_cart_ to represent moving a book. 
    working_cart_.pop();
  }

}

void Checkout::do_checkout() {

  // For each book in the checkout counter queue, find the book by ISBN in
  // the store's database.  If the book on the counter is found in the
  // database then accumulate the amount due and print the book's full
  // description and price on the receipt (i.e. write the book's full
  // description and price to standard output).  Otherwise, print a message
  // on the receipt that a description and price for the book wasn't found
  // and there will be no charge.

  // Create a const int to store the size of checkout_counter_.
  const int original_size = checkout_counter_.size();

  for (int i = 0; i < original_size; ++i) {
    // Create an std::string to store the isbn of the current book in checkout_counter_.
    std::string isbn = checkout_counter_.front().isbn();

    // Create a pointer to store the pointer that 
    // calling BookDatabase::find() returns. 
    // The find() function uses the store's database. 
    Book* book_ptr = book_database_.find(isbn);

    // If book_ptr does not return nullptr
    if (book_ptr != nullptr) {
      // Add the price of the book book_ptr is pointing
      // to. Dereference book_ptr to call its price function. 
      amount_due_ += (*book_ptr).price();
      
      // Print out the book's full description since 
      // it was found in the store's database. 
      std::cout << *book_ptr << std::endl;
    } else { /* Book was not found. */ 
      std::cout << "A description and price for \""
        << isbn << "\" not found, book is 0.0\n"
        << std::endl;
    }
    // Pop the front book. 
    checkout_counter_.pop();
  }

  std::cout << "-----------------------" << std::endl;
  
  // Print out the total.
  std::cout << "Total: $" << amount_due_ << std::endl;
}

std::stack<Book> Checkout::my_cart() const {
  return my_cart_;
}

std::stack<Book> Checkout::working_cart() const {
  return working_cart_;
}

std::queue<Book> Checkout::checkout_counter() const {
  return checkout_counter_;
}

double Checkout::amount_due() const {
  return amount_due_;
}