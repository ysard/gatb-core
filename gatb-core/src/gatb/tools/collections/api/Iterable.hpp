/*****************************************************************************
 *   GATB : Genome Assembly Tool Box
 *   Copyright (C) 2014  R.Chikhi, G.Rizk, E.Drezen
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

/** \file Iterable.hpp
 *  \date 01/03/2013
 *  \author edrezen
 *  \brief Iterable interface
 *
 *  This file holds interfaces related to the Iterable interface
 */

#ifndef _GATB_CORE_TOOLS_COLLECTIONS_ITERABLE_HPP_
#define _GATB_CORE_TOOLS_COLLECTIONS_ITERABLE_HPP_

/********************************************************************************/

#include <gatb/system/api/ISmartPointer.hpp>
#include <gatb/tools/designpattern/api/Iterator.hpp>

/********************************************************************************/
namespace gatb          {
namespace core          {
/** \brief Tools package */
namespace tools         {
/** \brief Collections interfaces */
namespace collections   {
/********************************************************************************/

/** \brief Iterable interface
 *
 * The Iterable interface provides an operation that creates an iterator.
 *
 * Note that one Iterable instance can create several iterators.
 */
template <class Item> class Iterable : public virtual system::ISmartPointer
{
public:

    virtual ~Iterable() {}

    /** Create an iterator for the given Iterable instance.
     * \return the new iterator. */
    virtual dp::Iterator<Item>* iterator () = 0;

    /** Return the number of items. If a specific implementation doesn't know the value,
     * it should return -1 by convention.
     * \return the number of items if known, -1 otherwise. */
    virtual int64_t getNbItems () = 0;

    /** Return a buffer of items.
     * \param[out] buffer : the buffer
     * \return the buffer */
    virtual Item* getItems (Item*& buffer)
    {
        throw "Iterable::getItems... SHOULD NOT BE HERE...";
        return buffer;
    }
};

/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/

#endif /* _GATB_CORE_TOOLS_COLLECTIONS_ITERABLE_HPP_ */
