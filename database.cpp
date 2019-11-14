// FILE: database.cpp
// CLASS implemented: database (see database.h for documentation)

// INVARIANT for the database ADT:
//   1. The items in the database are stored in a dynamic array, where each entry of the array includes
//      a company name, a pointer to the head of the linked list of products, and a
//      a pointer to the tail of the linked list of products
//   2. The number of slots in the array of companies is stored in member varibale aloc_slots
//   3. The number of used slots of the array of companies is stored in member varibale used_slots


#ifndef DATABASE_CPP
#define DATABASE_CPP

#include "database.h"
#include "company.h"
//#include "company.cpp"
//#include "node.h"
//#include "node.cpp"


//#define USEDEBUG

#ifdef USEDEBUG
#define Debug( x ) std::cout << x
#else
#define Debug( x )
#endif


namespace coen79_lab7
{
    
    database::database(const size_type& initial_capacity) {
        used_slots = 0;
        aloc_slots = initial_capacity;
        company_array = new company[aloc_slots];
    }
    
    
    database::database(const database &src) {
        Debug("Copy constructor..." << std::endl);
        // COMPLETE THE IMPLEMENTATION...

		this->used_slots = src.used_slots;
		this->aloc_slots = src.aloc_slots;

		company_array = new company[aloc_slots];

		for (int i = 0; i < aloc_slots; i++) {
			this->company_array[i] = src.company_array[i];
		}
		
    }
    
    
    database& database::operator= (const database &rhs) {
        Debug("Assignment operator..." << std::endl);

        // COMPLETE THE IMPLEMENTATION...
		this->used_slots = rhs.used_slots;
		this->aloc_slots = rhs.aloc_slots;

		reserve(aloc_slots);
		

		for (int i = 0; i < used_slots; i++) {
			this->company_array[i] = rhs.company_array[i];
		}

		return *this;
    }
    
    
    database::~database() {
        // COMPLETE THE IMPLEMENTATION...

		delete[] company_array;
    }
    
    
	void database::reserve(size_type new_capacity) {
		Debug("Reserve..." << std::endl);

		if (new_capacity == aloc_slots)
			return; // The allocated memory is already the right size.

		if (new_capacity < used_slots) {
			new_capacity = used_slots; // Can't allocate less than we are using.
			return;
		}

		//don't change used slots
		//this changes the size of the company_array and changes aloc_slots

		company* tmp = new company[new_capacity];

		std::copy(company_array, company_array + used_slots, tmp);

		delete[] company_array;
		company_array = tmp;
		aloc_slots = new_capacity;
		
    }
	//Postcondition: The database's current capacity is changed to the new_capacity (but not less than the number of items already in the database).
	//The insert_company function will work efficiently (without allocating new memory) until the new capacity is reached.
    
    
    bool database::insert_company(const std::string &entry) {
        
        Debug("Insert company..." << std::endl);

        assert(entry.length() > 0);
        
        size_type pos = search_company(entry);
        
        // If you find a company that is false, because there are duplicates
        if (pos != COMPANY_NOT_FOUND) {
            return false;
        }

        // COMPLETE THE IMPLEMENTATION...
		reserve(aloc_slots + 1);
		company_array[used_slots] = entry;
		used_slots++;

		return true;
    }
	//Postcondition: A new Company is added to the list
    
    
    bool database::insert_item(const std::string &company, const std::string &product_name, const float &price) {
        Debug("Insert item..." << std::endl);

        assert(company.length() > 0 && product_name.length() > 0);

        // COMPLETE THE IMPLEMENTATION...
		int loc = search_company(company);

		if (loc == COMPANY_NOT_FOUND) {
			return false;
		} else {
			
			if (list_contains_item(company_array[search_company(company)].get_head(), product_name)) {
				return false;
			} else {
				company_array[search_company(company)].insert(product_name, price);
			}
		}

		return true;
    }
	//Postcondition: A new product is added to the list pertaining to the company
	//This function also checks to see if the company is already in the database.
	//Returns false if the compnay is already in the database, otherwise returns true.
    
    
    bool database::erase_company(const std::string &company) {
		assert(company.length() > 0);
        size_type company_index = search_company(company);

		if (company_index == COMPANY_NOT_FOUND) {
			return false;
		} else {
			//delete all item ojects
			node *ptr = company_array[company_index].get_head();
			list_clear(ptr);
			free(ptr);
			
			//shift all other company objects over
			for(int i = company_index; i < used_slots - 1; i++) {
				company_array[i] = company_array[i + 1];
			}
			used_slots--;
		}
		return true;
    }

	//Postcondition:  A company (as well as all its products) are removed from the database.  All elements to the right of the company are shifted to the left.
	//Returns false if the company was not found in the list.
    
    
    bool database::erase_item(const std::string& cName, const std::string& pName) {
        
        assert(cName.length() > 0 && pName.length() > 0);

		size_type company_index = search_company(cName);

		if (company_index == COMPANY_NOT_FOUND) {
			return false;
		} else if (company_array[company_index].erase(pName) == false) {
			return false;
		}

		//check adustment of head and tail -- done within insert and erase
		return true;
    }
	//Postcondition:  A product is erased from the company in the database.  Returns false if the company or the item was not found.
     
    
    database::size_type database::search_company(const std::string& company) {
        assert(company.length() > 0);

        // COMPLETE THE IMPLEMENTATION...

		for (int i = 0; i < used_slots; i++) {
			if (company_array[i].get_name() == company) {
				return i;
			}
		}

		return COMPANY_NOT_FOUND;
    }

	//Postcondition:  Searches the DB for the company of the specified name.  Returns the position if found, but if not found, returns COMPANY_NOT_FOUND.
    
    bool database::print_items_by_company(const std::string& cName) {
        assert(cName.length() > 0);

        size_type c_index = search_company(cName);
        
        if (c_index == COMPANY_NOT_FOUND) {
            return false;
        }
        
        std::cout << "Printing the products of " << cName << ":"<< std::endl;
        company_array[c_index].print_items();
        std::cout << std::endl;
        
        return true;
    }
    
    void database::print_companies() {
        
        std::cout << "Company List" << std::endl;
        for (int i = 0; i < used_slots; i++) {
            std::cout << "- " << company_array[i].get_name() << std::endl;
        }
    }
}

#endif
