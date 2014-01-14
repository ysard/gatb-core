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

/** \file BankFasta.hpp
 *  \date 01/03/2013
 *  \author edrezen
 *  \brief Interface definition for genomic databases management
 */

#ifndef _GATB_CORE_BANK_IMPL_BANK_FASTA_HPP_
#define _GATB_CORE_BANK_IMPL_BANK_FASTA_HPP_

/********************************************************************************/

#include <gatb/bank/impl/AbstractBank.hpp>

#include <vector>
#include <string>

/********************************************************************************/
namespace gatb      {
/** \brief Core package of the GATP project.
 *
 * The gatb::core package holds all the fundamental packages needed for writting
 * assembly algorithms.
 *
 * It holds some generic tools, like operating system abstraction, collections management or design patterns
 * concerns. It also holds recurrent needs as reading genomic banks, handling kmers and so on.
 */
namespace core      {
/** \brief Package for genomic databases management. */
namespace bank      {
/** \brief Implementation for genomic databases management. */
namespace impl      {
/********************************************************************************/

/** \brief Interface for reading genomic databases.
 *
 * Sample of use:
 * \snippet bank1.cpp  snippet1_bank
 */
class BankFasta : public AbstractBank
{
public:

    /** Returns the name of the bank format. */
    static const char* name()  { return "fasta"; }

    /** Constructor.
     * \param[in] filenames : uri list of the banks. */
    BankFasta (const std::vector<std::string>& filenames);

    /** Constructor.
     * \param[in] argc : number of filenames
     * \param[in] argv : filenames
     */
    BankFasta (int argc, char* argv[]);

    /** Constructor.
     * \param[in] filename : uri of the bank. */
    BankFasta (const std::string& filename, bool output_fastq = false);


    /** Destructor. */
    ~BankFasta ();

    /** \copydoc IBank::getId. */
    std::string getId ()  { return _filenames[0]; }

    /** \copydoc IBank::iterator */
    tools::dp::Iterator<Sequence>* iterator ()  { return new Iterator (*this); }

    /** */
    int64_t getNbItems () { return -1; }

    /** \copydoc IBank::insert */
    void insert (const Sequence& item);

    /** \copydoc IBank::flush */
    void flush ()  {}

    /** \copydoc IBank::getSize */
    u_int64_t getSize ()  { return filesizes; }

    /** \copydoc IBank::estimate */
    void estimate (u_int64_t& number, u_int64_t& totalSize, u_int64_t& maxSize);

    /** \return maximum number of files. */
    static const size_t getMaxNbFiles ()  { return 30; }

    /************************************************************/

    /** \brief Specific Iterator impl for Bank class
     *
     * This implementation relies on the initial code from Minia. It wraps the
     * Iterator interface with the Minia code.
     *
     * Note that we made some effort not to put implementation code
     * here in the header; see in particular buffered_file and buffered_strings
     * attributes whose type is void* (and not the implementation type defined in
     * the cpp file).
     *
     * Note the small improvement compared to Minia: it is possible to create an
     * Iterator that provides (or not) sequence comments, according to the corresponding
     * parameter given to the Iterator constructor.
     *
     *  <b>IMPROVEMENTS</b>:
     *  - in case we have several banks to read, we could have at one time only one stream opened on the currently
     *  iterated file. The current implementation opens all streams, which may be avoided.
     */
    class Iterator : public tools::dp::Iterator<Sequence>
    {
    public:

        /** Define what kind of comment we want to retrieve. Such comments can be retrieved through
         * gatb::core::bank::Sequence
         */
        enum CommentMode_e
        {
            /** Empty comments are provided to clients. */
            NONE,
            /** Comments with only the FASTA ID are provided to clients. \n
             *  Ex: 'ENSTTRP00000009639pep:novel'*/
            IDONLY,
            /** Full comments are provided to clients. \n
             *  Ex: 'ENSTTRP00000001236pep:novel genescaffold:turTru1:GeneScaffold_3311:182575:189152:1'*/
            FULL
        };

        /** Constructor.
         * \param[in] ref : the associated iterable instance.
         * \param[in] commentMode : kind of comments we want to retrieve
         */
        Iterator (BankFasta& ref, CommentMode_e commentMode = FULL);

        /** Destructor */
        ~Iterator ();

        /** \copydoc tools::dp::Iterator::first */
        void first();

        /** \copydoc tools::dp::Iterator::next */
        void next();

        /** \copydoc tools::dp::Iterator::isDone */
        bool isDone ()  { return _isDone; }

        /** \copydoc tools::dp::Iterator::item */
        Sequence& item ()     { return *_item; }

        /** Estimation of the sequences information */
        void estimate (u_int64_t& number, u_int64_t& totalSize, u_int64_t& maxSize);

    private:

        /** Reference to the underlying Iterable instance. */
        BankFasta&    _ref;

        /** Tells what kind of comments we want as a client of the iterator. */
        CommentMode_e  _commentsMode;

        /** Tells whether the iteration is finished or not. */
        bool _isDone;

        /* Number of time next has been called   */
        
        u_int64_t   _nIters;
        
        /** Initialization method. */
        void init ();

        /** Finish method. */
        void finalize ();

        int   index_file; // index of current file

        void** buffered_file;
        void*  buffered_strings;   // variable_string_t *read, *dummy, *header;

        bool get_next_seq           (tools::misc::Vector<char>& data);
        bool get_next_seq           (tools::misc::Vector<char>& data, std::string& comment, std::string& quality, CommentMode_e mode);

        bool get_next_seq_from_file (tools::misc::Vector<char>& data, int file_id);
        bool get_next_seq_from_file (tools::misc::Vector<char>& data, std::string& comment, std::string& quality, int file_id, CommentMode_e mode);

        size_t _index;
    };

protected:

    friend class Iterator;

    bool _output_fastq;
    /** List of URI of the banks. */
    std::vector<std::string> _filenames;

    u_int64_t filesizes;  // estimate of total size for all files
    int       nb_files;   // total nb of files

    /** File handle for inserting sequences into the bank. */
    FILE* _insertHandle;

    /** Initialization method (compute the file sizes). */
    void init ();
};

/********************************************************************************/

/** */
class BankFastaFactory : public IBankFactory
{
public:

    IBank* createBank (const std::string& uri) { return new BankFasta (uri); }
};

/********************************************************************************/
} } } } /* end of namespaces. */
/********************************************************************************/

#endif /* _GATB_CORE_BANK_IMPL_BANK_FASTA_HPP_ */
