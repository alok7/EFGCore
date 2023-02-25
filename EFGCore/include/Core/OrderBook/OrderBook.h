#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <map>

namespace EFG { namespace Core {

  enum OrderBookSide{ask, bid};
  template<class KeyType, class ValType>
  class PriceDepthBook // L2 
  {
    private:
      std::map<KeyType, ValType, std::greater<KeyType>> mBids; 
      std::map<KeyType, ValType, std::less<KeyType>> mAsks; 
      template<typename _container_type>
      void _insert(_container_type& _container, const KeyType& _key, const ValType& _val) {
        _container.insert(_key, _val);
      }
      template<typename _container_type>
      void _update(_container_type& _container, const KeyType& _key, const ValType& _val) {
        _container[_key] = _val;
      }
      template<typename _container_type>
      bool _remove(_container_type& _container, const KeyType& _key) {
        return _container.erase(_key);
      }
      template<typename _container_type>
      void _remove(_container_type& _container, typename _container_type::iterator _it) {
        _container.erase(_it);
      }
      template<typename _container_type>
      void _sort(_container_type& _in_container, std::vector<ValType>& _out_container) {
        uint i = 0;
        for(auto it = _in_container.begin(); it != _in_container.end() && i < 10; it++) {
          _out_container.push_back(it->second); i++;
        }
      }
      template<typename _container_type>
      typename _container_type::iterator _find(_container_type& _container, const KeyType& _key) {
        typename _container_type::iterator it = _container.find(_key);
        return it;
      }
      template<typename _container_type>
      void _clear(_container_type& _container) {
        _container.clear();
      }
                
    public:
      PriceDepthBook() {}
      void insert(OrderBookSide side, const KeyType& key, const ValType& val) {
        switch (side) {
          case OrderBookSide::ask: this->_insert(this->mAsks, key, val, mAsks.end()); break;
          case OrderBookSide::bid: this->_insert(this->mBids, key, val, mBids.end()); break;
          default: std::cerr << "Error: invalid order type for insert!" << "\n";
        }
      }
      void insert(OrderBookSide side, const KeyType& key, const ValType& val) {
        // it is suggested that even for an update, a call to insert would do the job
        // inserts a key with the mapping val, if the key exists, then updates it
        switch (side) {
          case OrderBookSide::ask: this->_insert(this->mAsks, key, val); break;
          case OrderBookSide::bid: this->_insert(this->mBids, key, val); break;
          default: std::cerr << "Error: invalid order type for insert!" << "\n";
        }
      }
      void update(OrderBookSide side, const KeyType& key, const ValType& val) {
        switch (side) {
          case OrderBookSide::ask: this->_update(this->mAsks, key, val); break;
          case OrderBookSide::bid: this->_update(this->mBids, key, val); break;
          default: std::cerr << "Error: invalid order type for update!" << "\n";
        }
      }
      bool remove(OrderBookSide side, const KeyType& key) {
        // returns true if the key was erased, false if the key was not found
        switch(side) {
          case OrderBookSide::ask: return this->_remove(this->mAsks, key); break;
          case OrderBookSide::bid: return this->_remove(this->mBids, key); break;
          default: std::cerr << "Error: invalid order type for remove!" << "\n";
        }
      }
      template<typename Iterator>
      void remove(OrderBookSide side, Iterator it) {
        switch(side) {
          case OrderBookSide::ask: return this->_remove(this->mAsks, it); break;
          case OrderBookSide::bid: return this->_remove(this->mBids, it); break;
          default: std::cerr << "Error: invalid order type for remove!" << "\n";
        }
      }
      void clear() {
        this->_clear(this->mAsks);
        this->_clear(this->mBids);
      }
      void getAsks(std::vector<ValType>& out) {
        this->_sort(this->mAsks, out);
      }
      void getBids(std::vector<ValType>& out) {
        this->_sort(this->mBids, out);
      }
      template<typename Iterator>
      Iterator find(OrderBookSide side, const KeyType& key) {
        switch(side) {
          case OrderBookSide::ask: return this->mAsks.find(key); break;
          case OrderBookSide::bid: return this->mBids.find(key); break;
        }
      }
      template<typename Iterator>
      Iterator lowerBound(OrderBookSide side, const KeyType& key) {
        switch(side) {
          case OrderBookSide::ask: return this->mAsks.lowerBound(key); break;
          case OrderBookSide::bid: return this->mBids.lowerBound(key); break;
        }
      }
      template<typename Iterator>
      Iterator begin(OrderBookSide side) {
        switch(side) {
          case OrderBookSide::ask: return this->mAsks.begin(); break;
          case OrderBookSide::bid: return this->mBids.begin(); break;
        }
      }
      template<typename Iterator>
      Iterator end(OrderBookSide side) {
        switch(side) {
          case OrderBookSide::ask: return this->mAsks.end(); break;
          case OrderBookSide::bid: return this->mBids.end(); break;
        }
      }
      bool exists(OrderBookSide side, const KeyType& key) {
        switch(side) {
          case OrderBookSide::ask: return this->mAsks.find(key)!=mAsks.end(); break;
          case OrderBookSide::bid: return this->mBids.find(key)!=mBids.end(); break;
        }
      }
  };

class PriceDepthBookL3
{

};

}}

#endif

