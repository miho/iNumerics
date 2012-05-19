/// \file   inmemcollect.cpp
/// \author Michael Hoffer
/// \date   2012
/// \brief Contains the definition of the memory management.

#define INMEMCOLLECT_CPP
#include "inmemcollect.h"

//#define DEBUG 0


namespace iNumerics {

    /******************************************************************************
     *	                                                                          *
     *       Class: MemCollect (PUBLIC-MEMBERS)                                    *
     *                                                                             *
     ******************************************************************************/

    template <class T>
    MemCollect<T>::MemCollect() {
        // Debug-Output
        IN_DISPLAY("MemCollect<T>::MemCollect ( )", 1)
        // Initializing member variables
        _maxMem = 0;
        _memTolerance = 0;
        _memListSize = 0;
        _initialized = false;
    }

    template <class T>
    void MemCollect<T>::initialize(Byte maxMem, Byte memTolerance) {
        if (_initialized) {
            IN_DISPLAY("MemCollect<T>::initialize( Byte maxMem, Byte memTolerance ): already initialized! Ignoring method call.", 1);
            return;
        }

        // Debug-Output
        IN_DISPLAY("MemCollect<T>::initialize( Byte maxMem, Byte memTolerance )", 1);
        _maxMem = maxMem;
        _memTolerance = memTolerance;
        _initialized = true;
    }

    template <class T>
    void MemCollect<T>::allocMem(MemType<T>*& mem, inULong n) {
        // Debug-Output
        IN_DISPLAY("MemCollect<T>::allocMem(inULong n)", 1);


        if (!_initialized) {
            std::cerr << "MemCollect<T>:" << std::endl;
            std::cerr << ">> ERROR: MemCollect is not initialized!" << std::endl;
            std::cerr << ">> Please call MemCollect<T>::initialize( Byte maxMem, Byte memTolerance ) before usage!" << std::endl;
            exit(-1);
        }

        bool foundMem = false;

        if (_freeMemList.size() != 0) {
            // Debug-Output
            IN_DISPLAY(">> WE HAVE A MEMLIST! Size=" << _freeMemList.size() << " (" << iNumerics::MByte(_memListSize).value() << " MByte)", 2);

            typename std::deque< MemType<T>* >::iterator i;


            // Iterate through freeMemList and search for matching entry
            for (i = _freeMemList.begin(); i != _freeMemList.end(); i++) {
                iNumerics::Byte sizeOfType = iNumerics::Byte(n * sizeof ( T));

                iNumerics::Byte sizeOfI = (*i) ->memSize();

                if (sizeOfType.value() > sizeOfI.value()) {
                    continue;
                }

                // Compute difference between size of list-element *i and requested memory
                iNumerics::Byte TOL = iNumerics::Byte(sizeOfI.value() - sizeOfType.value());

                // If memory does match, detach it from freeMemList and hand it over.
                if (TOL.value() <= _memTolerance.value()) {
                    mem = *i;
                    detachFromMemList(i);
                    foundMem = true;

                    // Debug-Output
                    IN_DISPLAY(">> FOUND MEM in List", 2);

                    // We don't need to iterate through freeMemList anymore.
                    break;
                }
            }
        }
        
        // If no entry has been found, allocate new memory.
        if (!foundMem) {
            mem = new MemType<T > (n);
        }

        // Don't forget to initialize newly allocated memory.
        mem->zero();

        // Now a new object is referencing mem.
        mem->addReference();

        // Debug-Output
        IN_DISPLAY(">> &MEM:" << mem, 2);
    }

    template <class T>
    void MemCollect<T>::freeMem(MemType<T>*& mem) {
        // Debug-Output
        IN_DISPLAY("MemCollect<T>::freeMem( MemType<T>*& mem )", 1);

        if (mem != NULL) {
            // Only remove reference if it isn't already in freeMemList
            // as only memory-entries with no references are added to
            // freeMemlist
            if (!isInList(mem)) {
                mem->removeReference();
            }

            // Debug-Output
            IN_DISPLAY(">> References to Memory:" << mem->getRefCount(), 2);

            if (!mem->isReferenced()) {
                // If mem is managed manually, we can't add it to
                // freeMemList as we don't know if it's in use or not.
                if (!mem->manualMemoryManagement()) {
                    attachToMemList(mem);
                } else {
                    // Debug-Output
                    IN_DISPLAY(">> Memory is managed manually. Memory won't be added to List!", 2);

                    // Really delete memory, i.e. delete the MemType object which handles the memory.
                    // The memory itself won't be deleted.
                    delMem(mem);
                };

                // Debug-Output
                IN_DISPLAY(">> List-Size: " << _freeMemList.size() << " (" << iNumerics::MByte(_memListSize).value() << " MByte)", 2);

                if (_freeMemList.size() > 0) {
                    typename std::deque< MemType<T>* >::iterator i = _freeMemList.end();

                    // Remove elements from freeMemList as long as it is to big.
                    while (_memListSize.value() > _maxMem.value()) {
                        // Debug-Output
                        IN_DISPLAY(">> LIST to BIG, Size:" << iNumerics::MByte(_memListSize).value() << " MByte)", 2);

                        std::cout << ">> LIST to BIG, Size:" << iNumerics::MByte(_memListSize).value() << " MByte)\n";

                        --i;

                        if (i == _freeMemList.begin()) {
                            IN_DISPLAY(">> LIST emptied.", 2);
                            break;
                        };

                        MemType<T>* tmpMem;
                        tmpMem = *i;
                        delMem(tmpMem);
                    }
                }
            }
        }
    }

    template <class T>
    void MemCollect<T>::freeAll() {
        while (_freeMemList.size() > 0) {
            delMem(*_freeMemList.begin());
        }
    }

    template <class T>
    bool MemCollect<T>::isInList(MemType<T>* mem) {
        // Checks weather _mem is already in freeMemList
        bool identical = false;
        typename std::deque<MemType<T>* >::iterator i;
        for (i = _freeMemList.begin(); i != _freeMemList.end(); i++) {
            if (mem == *i) {
                identical = true;
            }
        }
        return identical;
    }

    template <class T>
    MemCollect<T>::~MemCollect() {
        // Debug-Output
        IN_DISPLAY("MemCollect<T>::Destructor()", 1);

        freeAll();
    }

    /******************************************************************************
     *	                                                                          *
     *       Class: MemCollect (PRIVATE-MEMBERS)                                   *
     *                                                                             *
     ******************************************************************************/

    template <class T>
    void MemCollect<T>::attachToMemList(MemType<T>* mem) {
        // Debug-Output
        IN_DISPLAY("MemCollect<T>::attachToMemList ( MemType<T>* mem )", 1);
        if (mem != NULL) {
            if (!isInList(mem)) {
                _freeMemList.push_front(mem);
                _memListSize += mem->memSize();
            } else {
                // Debug-Output
                IN_DISPLAY(">> WARNING: mem already in freeMemList!", 1);
            }
        } else {
            // Debug-Output
            IN_DISPLAY(">> WARNING: mem == NULL. Element not added to freeMemList!", 1);
        }
    }

    template <class T>
    void MemCollect<T>::detachFromMemList(typename std::deque< MemType<T>* >::iterator i) {
        // Debug-Output
        IN_DISPLAY("MemCollect<T>::detachFromMemList ( typename deque< MemType<T>* >::iterator i )", 1);

        if (*i != NULL) {
            inLong value = _memListSize.value() - (*i)->memSize().value();
            //std::cout << "----------*** SIZE: " << value << std::endl;
            IN_ASSERT(value >= 0, 0);

            //if (value>=0) {
            _freeMemList.erase(i);
            _memListSize = Byte(value);
            //}

            IN_ASSERT(_memListSize.value() >= 0, 0);
        } else {
            // Debug-Output
            //IN_DISPLAY ( ">> WARNING: *i == NULL. Element not removed from freeMemList!",1 );
            IN_DISPLAY("-------------------------------------------------- >> WARNING: *i == NULL. Element not removed from freeMemList!", 1);
        }
    }

    template <class T>
    void MemCollect<T>::delMem(MemType<T>*& mem) {
        bool foundMem = false;

        // Debug-Output
        IN_DISPLAY("MemCollect<T>::delMem( T* &mem )", 1);

        if (mem != NULL) {
            // Try to find corresponding entry in freeMemList and remove it before deleting memory.
            for (typename std::deque< MemType<T>* >::iterator i = _freeMemList.begin(); i != _freeMemList.end(); i++) {
                if (*i == mem) {
                    detachFromMemList(i);

                    // Debug-Output
                    IN_DISPLAY(">> List-Size: " << _freeMemList.size() << " (" << MByte(_memListSize).value() << " MByte)", 2);

                    foundMem = true;
                    break;
                }
            }

            delete mem;
            mem = NULL;

            if (!foundMem) {
                //Debug-Output
                IN_DISPLAY(">> WARNING: could not find corresponding entry in freeMemList!", 2);
            }
        } else {
            //Debug-Output
            IN_DISPLAY(">> WARNING: given parameter is a zero-pointer!", 1);
        }
    }

}
