#include <cassert>
#include <iomanip>
#include <iostream>

#include "History.hpp"
#include "Transaction.hpp"
#include "project4.hpp"

//
// Definitions for Transaction class
//

// Constructor
// TASK 1
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date,
                         unsigned int month_date, unsigned year_date,
                         bool buy_sell_trans, unsigned int number_shares,
                         double trans_amount)
    : symbol{ticker_symbol}, day{day_date}, month{month_date}, year{year_date},
      trans_type{(buy_sell_trans) ? "Buy" : "Sell"}, shares{number_shares},
      amount{trans_amount}, trans_id{assigned_trans_id++}, p_next{nullptr}, acb{}, acb_per_share{}, share_balance{}, cgl{} {}

// Destructor
// TASK 1
Transaction::~Transaction()
{
  symbol = "0";
  day = 0;
  month = 0;
  year = 0;
  trans_type = "0";
  shares = 0;
  amount = 0;
  trans_id = 0;
  acb = 0;
  acb_per_share = 0;
  share_balance = 0;
  cgl = 0;
  assigned_trans_id = 0;
  p_next = nullptr;
}

// Overloaded < operator.
// TASK 2
bool Transaction::operator<(Transaction const &other)
{
  if ((this->year == other.year) && (this->month == other.month) &&
      (this->day == other.day))
  {
    return ((this->trans_id) > (other.trans_id)) ? true : false;
  }

  if (this->year > other.year)
  {
    return false;
  }
  if (this->year < other.year)
  {
    return true;
  }

  if (this->month > other.month)
  {
    return false;
  }

  if (this->month < other.month)
  {
    return true;
  }

  if (this->month == other.month)
  {
    if (this->day > other.day)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  assert(false);
  return false;
}

// GIVEN
// Member functions to get values.

std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const
{
  return (trans_type == "Buy") ? true : false;
}
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb(double acb_value) { acb = acb_value; }
void Transaction::set_acb_per_share(double acb_share_value)
{
  acb_per_share = acb_share_value;
}
void Transaction::set_share_balance(unsigned int bal) { share_balance = bal; }
void Transaction::set_cgl(double value) { cgl = value; }
void Transaction::set_next(Transaction *p_new_next) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print()
{
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " " << std::setw(4)
            << get_symbol() << " " << std::setw(4) << get_day() << " "
            << std::setw(4) << get_month() << " " << std::setw(4) << get_year()
            << " ";

  if (get_trans_type())
  {
    std::cout << "  Buy  ";
  }
  else
  {
    std::cout << "  Sell ";
  }

  std::cout << std::setw(4) << get_shares() << " " << std::setw(10)
            << get_amount() << " " << std::setw(10) << get_acb() << " "
            << std::setw(4) << get_share_balance() << " " << std::setw(10)
            << std::setprecision(3) << get_acb_per_share() << " "
            << std::setw(10) << std::setprecision(3) << get_cgl() << std::endl;
}

//
// Definitions for the History class
//

// Constructor
// TASK 3
History::History() : p_head{nullptr} {}

// Destructor
// TASK 3
History::~History()
{
  Transaction *old{p_head};
  while (p_head != nullptr)
  {
    old = p_head;
    p_head = p_head->get_next();
    delete old;
    old = nullptr;
  }
}

// read_history(...): Read the transaction history from file.
// TASK 4-NOT
void History::read_history()
{
  ece150::open_file();
  while (ece150::next_trans_entry())
  {
    Transaction *new_trans{new Transaction{ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount()}};
    insert(new_trans);
  }
  ece150::close_file();
}

// insert(...): Insert transaction into linked list.
// TASK 5
Transaction *read_list(Transaction *pointer);
void History::insert(Transaction *p_new_trans)
{
  if (this->p_head == nullptr)
  {
    (this->p_head) = p_new_trans;
  }
  else
  {
    read_list(this->p_head)->set_next(p_new_trans);
  }
}

Transaction *read_list(Transaction *pointer)
{
  Transaction *ans{};
  while ((pointer->get_next()) != nullptr)
  {
    ans = pointer;
    pointer = pointer->get_next();
  }
  ans = pointer;
  return ans;
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6-NOT
int count_trans(Transaction *Head);
void exchange(Transaction *&left, Transaction *&right);
bool is_sorted(Transaction *array[], int size);
void History::sort_by_date()
{
  int capacity{count_trans(this->get_p_head()) - 1};
  Transaction **total_pointers{new Transaction *[capacity]};
  total_pointers[0] = this->get_p_head();
  total_pointers[capacity - 1] = nullptr;
  for (int i{1}; i < capacity; ++i)
  {
    total_pointers[i] = total_pointers[i - 1]->get_next();
  }
  while (is_sorted(total_pointers, capacity))
  {
    int k = 0;
    for (; k < capacity - 1; ++k)
    {
      if (!((*(total_pointers[k])) < (*(total_pointers[k + 1]))))
      {
        exchange(total_pointers[k], total_pointers[k + 1]);
      }
    }
  }
  // start changing
  p_head = total_pointers[0];
  for (int i{1}; i < capacity; ++i)
  {
    total_pointers[i - 1]->set_next(total_pointers[i]);
    total_pointers[i - 1] = nullptr;
  }
  total_pointers[capacity - 1]->set_next(nullptr);
  total_pointers[capacity - 1] = nullptr;
  // delete memory
  delete[] total_pointers;
  total_pointers = nullptr;
}

bool is_sorted(Transaction *array[], int capacity)
{
  for (int i{capacity - 1}; i > 0; --i)
  {
    if (!((*array[i - 1]) < (*array[i])))
    {
      return true;
    }
  }
  return false;
}

void exchange(Transaction *&left, Transaction *&right)
{
  Transaction *tmp{left};
  left = right;
  right = tmp;
}
int count_trans(Transaction *Head)
{
  int ans{1};
  Transaction *current{Head};
  while (current != nullptr)
  {
    current = current->get_next();
    ++ans;
  }
  return ans;
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7

void History::update_acb_cgl()
{
  Transaction *pre_current{get_p_head()};
  Transaction *current{get_p_head()->get_next()};
  int capacity{count_trans(current)};

  pre_current->set_acb(pre_current->get_amount());
  pre_current->set_acb_per_share((pre_current->get_amount()) / (pre_current->get_shares()));
  pre_current->set_share_balance(pre_current->get_shares());
  pre_current->set_cgl(0);

  for (int i{1}; i < capacity; ++i)
  {
    current->set_share_balance(current->get_trans_type() ? (current->get_shares() + pre_current->get_share_balance()) : (pre_current->get_share_balance() - current->get_shares()));
    current->set_acb_per_share(current->get_trans_type() ? (pre_current->get_share_balance() * pre_current->get_acb_per_share() + current->get_amount()) / current->get_share_balance() : pre_current->get_acb_per_share());
    current->set_cgl(current->get_trans_type() ? 0 : (current->get_amount() - current->get_shares() * current->get_acb_per_share()));
    current->set_acb(current->get_trans_type() ? (current->get_amount() + pre_current->get_acb()) : (pre_current->get_acb() - current->get_amount() + current->get_cgl()));

    pre_current = current;
    current = current->get_next();
  }
}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year)
{
  // update_acb_cgl();
  Transaction *fp{get_p_head()};
  double ans{};

  while (fp != nullptr)
  {
    if (fp->get_year() == year)
    {
      ans += fp->get_cgl();
    }
    fp = fp->get_next();
  }
  return ans;
}

// print() Print the transaction history.
// TASK 9
void History::print()
{
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
  Transaction *pointer{this->get_p_head()};
  while (pointer != nullptr)
  {
    pointer->print();
    pointer = pointer->get_next();
  }
  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}

// GIVEN
// get_p_head(): Full access to the linked list.
Transaction *History::get_p_head() { return p_head; }