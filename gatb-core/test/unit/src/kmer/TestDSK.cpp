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

#include <CppunitCommon.hpp>

#include <gatb/system/impl/System.hpp>

#include <gatb/bank/impl/Banks.hpp>

#include <gatb/kmer/impl/SortingCountAlgorithm.hpp>

#include <gatb/tools/misc/api/Macros.hpp>
#include <gatb/tools/misc/impl/Property.hpp>

#include <gatb/tools/designpattern/impl/IteratorHelpers.hpp>

#include <gatb/tools/math/LargeInt.hpp>
#include <gatb/tools/math/Integer.hpp>

#include <gatb/tools/storage/impl/Storage.hpp>

#include <boost/variant.hpp>

using namespace std;

using namespace gatb::core::system;
using namespace gatb::core::system::impl;

using namespace gatb::core::bank;
using namespace gatb::core::bank::impl;

using namespace gatb::core::kmer;
using namespace gatb::core::kmer::impl;

using namespace gatb::core::tools::dp;
using namespace gatb::core::tools::dp::impl;

using namespace gatb::core::tools::collections;
using namespace gatb::core::tools::collections::impl;

using namespace gatb::core::tools::storage;
using namespace gatb::core::tools::storage::impl;

using namespace gatb::core::tools::math;
using namespace gatb::core::tools::misc;
using namespace gatb::core::tools::misc::impl;

/********************************************************************************/
namespace gatb  {  namespace tests  {
/********************************************************************************/

struct Functor_getValue : public boost::static_visitor<Integer>    {
    template<typename T>  Integer operator() (const T& a) const  { return Integer(a.getValue());  }};

typedef boost::variant <
    Kmer<32>::Count,
    Kmer<64>::Count,
    Kmer<96>::Count,
    Kmer<128>::Count
>  KmerVariant;

/** \brief Test class for genomic databases management
 */
class TestDSK : public Test
{
    /********************************************************************************/
    CPPUNIT_TEST_SUITE_GATB (TestDSK);

        CPPUNIT_TEST_GATB (DSK_check1);
        CPPUNIT_TEST_GATB (DSK_check2);
        CPPUNIT_TEST_GATB (DSK_check3);

    CPPUNIT_TEST_SUITE_GATB_END();

public:

    /********************************************************************************/
    void setUp    () {}
    void tearDown () {}

    /********************************************************************************/
    void DSK_check1_aux (const char* sequences[], size_t nbSequences, size_t kmerSize, size_t nks, size_t checkNbSolids)
    {
        /** We create a storage instance. */
        Storage storage (STORAGE_FILE, "test");

        /** We create a DSK instance. */
        SortingCountAlgorithm<> dsk (&storage, new BankStrings (sequences, nbSequences), kmerSize, nks);

        /** We launch DSK. */
        dsk.execute();

        CPPUNIT_ASSERT (checkNbSolids == dsk.getInfo()->getInt("kmers_nb_solid"));
    }

    /********************************************************************************/
    void DSK_check1 ()
    {
        const char* s1 = "GATCCTCCCCAGGCCCCTACACCCAAT" ;

        const char* seqs1[] =  { s1 };
        DSK_check1_aux (seqs1, ARRAY_SIZE(seqs1), 27, 1, 1);
        DSK_check1_aux (seqs1, ARRAY_SIZE(seqs1), 26, 1, 2);
        DSK_check1_aux (seqs1, ARRAY_SIZE(seqs1), 27, 2, 0);
        DSK_check1_aux (seqs1, ARRAY_SIZE(seqs1), 26, 2, 0);

        const char* seqs2[] =  { s1, s1 };
        DSK_check1_aux (seqs2, ARRAY_SIZE(seqs2), 27, 1, 1);
        DSK_check1_aux (seqs2, ARRAY_SIZE(seqs2), 26, 1, 2);
        DSK_check1_aux (seqs2, ARRAY_SIZE(seqs2), 27, 2, 1);
        DSK_check1_aux (seqs2, ARRAY_SIZE(seqs2), 26, 2, 2);
        DSK_check1_aux (seqs2, ARRAY_SIZE(seqs2), 27, 3, 0);
        DSK_check1_aux (seqs2, ARRAY_SIZE(seqs2), 26, 3, 0);

        const char* seqs3[] =  { s1, s1, s1};
        DSK_check1_aux (seqs3, ARRAY_SIZE(seqs3), 27, 1, 1);
        DSK_check1_aux (seqs3, ARRAY_SIZE(seqs3), 26, 1, 2);
        DSK_check1_aux (seqs3, ARRAY_SIZE(seqs3), 27, 2, 1);
        DSK_check1_aux (seqs3, ARRAY_SIZE(seqs3), 26, 2, 2);
        DSK_check1_aux (seqs3, ARRAY_SIZE(seqs3), 27, 3, 1);
        DSK_check1_aux (seqs3, ARRAY_SIZE(seqs3), 26, 3, 2);
        DSK_check1_aux (seqs3, ARRAY_SIZE(seqs3), 27, 4, 0);
        DSK_check1_aux (seqs3, ARRAY_SIZE(seqs3), 26, 4, 0);

        const char* seqs4[] = {

            "CGCTACAGCAGCTAGTTCATCATTGTTTATCAATGATAAAATATAATAAGCTAAAAGGAAACTATAAATA"
            "ACCATGTATAATTATAAGTAGGTACCTATTTTTTTATTTTAAACTGAAATTCAATATTATATAGGCAAAG"
            "ACTTAGATGTAAGATTTCGAAGACTTGGATGTAAACAACAAATAAGATAATAACCATAAAAATAGAAATG"
            "AACGATATTAAAATTAAAAAATACGAAAAAACTAACACGTATTGTGTCCAATAAATTCGATTTGATAATT"
            "AGGTAACAATTTAACGTTAAAACCTATTCTTTTATTATCCGAAAATCCGTCGTGGAATTTGTATTAGCTT"
            "TTTTTCTACATTACCCGTTTGCGAGACAGGTGGGGTCAGACGTAGACGTAGTCTCTGGAGTCAAGACGAA"
            "ATTTTACATTTCACAATTTCCTATAGGCCGAGCAAAATTTATTAAGAACCCACAGGCATCATTACGTTTT"
            "CTTGCACAGAAGACTTCACGCTGAAGTCATTGGGCTATATTTCAACGAGACGTCTGTTGGTTTATAAAGG"
            "GCTATATTTATACAAGAATAGGAGTATGGCAGTATGCTAGGCTGGTATGTAGTACGTATACCTCCTAAGC"
            "CGAAAGGCAGTAAGTGACGATGTAATAGTTTTGAGGAAAATTACTTTTTCTGAATAATATTTTTATTTTT"
            "GTTTGCATTTTGTTAAAATTATTTACTAAATTAATGATTCTCATATGTTTTTTCATAGATTTGATGAACT"
            "ACTGTACCATCTGATTAGCGCATGGTCATAGCTGTTTCCTGTGTGAAATTGTTATCCGCTCACAATTCCA"
            "CACAACATACGAGCCGGAGCATAAAGTGTAAAGCCTGGGGTGCCTAATGAGTGAGCTACTCACATAATTG"
            "CGTGCGCTCACTGCCCGCTTTCCAGTCGGAAACCTGTCGTGCCAGCTGCATTATTGATCCGCCACGCCCC"
            "GGGGAAAGT",

            "GTCTTCATTCAGCTGTTCTCATGATAACTAGTAATTCCTTGCTAACAATTTTTACTGAGTAGCAACCAAT"
            "TAATGTTGCCAGAATTTCATAATTGAATTTGAATTTTTTATTTTTTCCTTGATTATGCTTCAAACTCTAT"
            "GTAGTTATTTAGAGTCAATAATATTAAAGCAATCTTAATATTAACTCATTTATTTCTGATTGGCCATATT"
            "TATTTAATTCTCAACAATAATAATGATAAGTATAATAATATATTTAACTTAATAACATTTTAATCATTTT"
            "ATTTTTGTTTGTTGTGATTTTTGGACGTTGTGGTAAATAAGAAGTTTTAAGCTTATATTAATATGTTTTA"
            "CTTTTTATTTCTTAATACGAATTTAATTACCTACCCATTATATTAAGTATATGTTTTGGAATTCTTTCTG"
            "TAAAAATGTGTTTTAAATATTTTACACTTAATTATGTAGGTACCTATACATTTTTAAACTTATCGTATAA"
            "TTCTTTTAATGGTTAAATCATACAAATTAATGTGTAGAGAATAGTTTTTATAAGACTCGTTGTCAATACG"
            "TACGCATAATATAAAAAAACTGACATGTTTTAGTAAGTCGTTTTGATGCATAATAGGATTTTTACCTTTT"
            "AAAGTCTCAAGTTTTCATACAGTGGTACCTCTATATAGAACACGTTAGGCTTTACGGGGTCATTATTTCT"
            "GTTCCGATATTTTTAATGGCATAAAACTATAAACAATAACCGGTATGTATAAATGGTAC",

            "ACCTGAAGCAGTAGTTCATCATATTGCGACTGCAGAATCGATGATAAAGTGGCTTTTAGATCTAAAAGCC"
            "AATACAAAACTGAAGGAATTTGATTTGATGGATTTTAATTTTGAAAATGGATTATGATTGTCGATTGATT"
            "AACAAGTTTACTAGGTTTGAATAGAGGTGATTCTTAATATTTCAAATATTTGAATGTCATGATGAATATT"
            "ATAATTTATAATTAAAAAATATCATATTTTATTCATGGATATCAAAGCTGAAAAAATAGATATTCAAAAT"
            "CGCCTTTATAATAACCTATCATAAACTAATTAATCAATTAAATTCAGTTTTAAAAATTTAAATCCGACAA"
            "ATAAAATTCCTTCAGCTCTGTCTGGGATTTTGGTCGAAAAATTTTAAATCGAAAAAAGTTTATCTTATTC"
            "ATAATATCATTGCCAATGATATTAAAATTAATTAACAACGAATACAAATAACGTCCGACCTGTATATTGC"
            "GGGCCAACTGTTTTTATAGGAAATGTTGACCGAAAACTATTACAGATTAGATGTGTGTGTGTTTACCCTG"
            "TACAAAAATACAAGTACTATTACAACACATCATTATGTTAAATTGCCTCTATATTAATTTCTTTAAAACA"
            "CGACCAACTGCACATTAAAGTAGTTTATTTAGTACTACAGTAGATTAAATTCATTTTTGACGAAAAATTG"
            "CATTTGAAAATGGCCATTGTGTGTATAAATATTGTATACTAATATAACTCTAAATAAAGGTTTCCAGTAC"
            "CAAAGAACCAAATTTTTAATTACAACCAAAATAACTAAATCGTATTCTTTGTTAAATAGTTAAGTTTTTC"
            "GCCGATTGCTGTGCTTGACAGTCTCCTCAATTCAGAATTTCATGTAAAATAAAAATAGCGTACATATAAT"
            "GGATTGCTGTGGCATTTGGTTTGATTAATCCCAAATATTGATTCCAAATATCTATTAGCCTATTGTACCC"
            "CGGAGTACCG"
        } ;

        /** The following solid kmers numbers are computed with the original minia. */
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 9, 1, 2540);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 9, 2, 151);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 9, 3, 18);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 9, 4, 3);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 9, 5, 2);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 9, 6, 0);

        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 11, 1, 2667);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 11, 2, 41);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 11, 3, 0);

        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 13, 1, 2690);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 13, 2, 12);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 13, 3, 0);

        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 15, 1, 2691);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 15, 2, 5);
        DSK_check1_aux (seqs4, ARRAY_SIZE(seqs4), 15, 3, 0);
    }

    /********************************************************************************/
    template<size_t span>
    void DSK_check2_aux ()
    {
        /** Shortcut. */
        typedef typename Kmer<span>::Type  Type;
        typedef typename Kmer<span>::Count Count;

        size_t kmerSize = 31;
        size_t nks      = 1;

        const char* s1 = "GATCGATTCTTAGCACGTCCCCCCCTACACCCAAT" ;

        /** We create a storage instance. */
        Storage storage (STORAGE_FILE, "test");

        /** We create a DSK instance. */
        SortingCountAlgorithm<span> sortingCount (&storage, new BankStrings (s1, 0), kmerSize, nks);

        /** We launch DSK. */
        sortingCount.execute();

        /** We iterate the solid kmers. */
        Iterator<Count>* iter = sortingCount.getSolidKmers()->iterator();
        LOCAL (iter);

        /** The following values have been computed with the original DSK.
         *  Note: we use a set and check that the solid iterator items are in this trustful set.
         *  -> we have to do this because we are not sure about the order of the iterated items.
         */
        set<Type> okValues;
        okValues.insert (0x1CA68D1E55561150);
        okValues.insert (0x09CA68D1E5556115);
        okValues.insert (0x2729A34795558454);
        okValues.insert (0x32729A3479555845);
        okValues.insert (0x0AFEE3FFF1ED8309);

        set<Type> checkValues;
        Type checksum = 0;

        size_t idx=0;
        for (iter->first(); !iter->isDone(); iter->next(), idx++)
        {
            typename set<Type>::iterator lookup = okValues.find (iter->item().value);
            CPPUNIT_ASSERT (lookup != okValues.end());

            checkValues.insert (iter->item().value);

            checksum += iter->item().value;
        }

        CPPUNIT_ASSERT (checksum == 0x8b0c176c3b43d207);
        CPPUNIT_ASSERT (checkValues.size() == okValues.size());

        /** We check the result through the variant type. */
        IteratorVariant <IteratorFile, Kmer<32>::Count, Kmer<64>::Count, Kmer<96>::Count, Kmer<128>::Count > itVariant;

        IteratorFile<Count>* solid = dynamic_cast<IteratorFile<Count>*> (iter);
        CPPUNIT_ASSERT (solid != 0);

        /** We set the variant with the current T type. */
        itVariant = *solid;

        Integer checksumGeneric (Type(0));

        for (itVariant.first(); !itVariant.isDone(); itVariant.next())
        {
            auto current = itVariant.item();

            Integer val = boost::apply_visitor (Functor_getValue(),  current);

            checksumGeneric += val;
        }
        CPPUNIT_ASSERT (checksumGeneric == Integer(Type(0x8b0c176c3b43d207)));
    }

    /********************************************************************************/
    void DSK_check2 ()
    {
        DSK_check2_aux<32> ();
        DSK_check2_aux<64> ();
        DSK_check2_aux<96> ();
    }

    /********************************************************************************/

    template<size_t span>
    void DSK_check3_aux (IBank* bank, size_t kmerSize, size_t nks)
    {
        /** Shortcut. */
        typedef typename Kmer<span>::Type  Type;
        typedef typename Kmer<span>::Count Count;

        LOCAL (bank);

        /** We set the default Integer precision. */
        Integer::setType (sizeof(Type)/8);

        TimeInfo ti;

        /** We create a storage instance. */
        Storage storage (STORAGE_FILE, "test");

        /** We create a DSK instance. */
        SortingCountAlgorithm<span> sortingCount (&storage, bank, kmerSize, nks);

        /** We launch DSK. */
        sortingCount.execute();

        /** We iterate the solid kmers. */
        Iterator<Count>* iter = sortingCount.getSolidKmers()->iterator();
        LOCAL (iter);

        // cout << "----------------------------------------------------------" << endl;

        /** We check the result through the variant type. */
        IteratorVariant <IteratorFile, Kmer<32>::Count, Kmer<64>::Count, Kmer<96>::Count, Kmer<128>::Count > itVar;

        IteratorFile<Count>* solid = dynamic_cast<IteratorFile<Count>*> (iter);
        CPPUNIT_ASSERT (solid != 0);

        /** We set the variant with the current T type. */
        itVar = *solid;

        Type    checksum1 = 0;
        Integer checksum2 = Integer(Type(0));

        PairedIterator <IteratorFile, Count, KmerVariant> itBoth (*solid, itVar);
        for (itBoth.first(); !itBoth.isDone(); itBoth.next())
        {
            Type    v1 = itBoth.item().first.getValue();
            Integer v2 = boost::apply_visitor (Functor_getValue(),  itBoth.item().second);

            checksum1 += (hash1 (v1,0) & 0x11111);
            checksum2 += Integer(Type((hash1 (v2,0) & 0x11111)));

             //cout << "["  << (Integer(v1) == v2 ? 'X' : ' ')  << "] "  << v1 << "  " <<  v2 << endl;
        }

        CPPUNIT_ASSERT (checksum2 == Integer(checksum1));

        /** Some performance tests. */

        size_t idx2 = 0;
        size_t idx1 = 0;

        { TIME_INFO(ti,"1");  iter->iterate ([&idx1](const Count&       k) { idx1++; });  }
        { TIME_INFO(ti,"2");  itVar.iterate ([&idx2](const KmerVariant& k) { idx2++; });  }
    }

    /** */
    void DSK_check3 ()
    {
        size_t kmerSize = 15;
        size_t nks      = 1;

        IBank* bank = new BankStrings (
            "CGCTACAGCAGCTAGTTCATCATTGTTTATCAATGATAAAATATAATAAGCTAAAAGGAAACTATAAATA"
            "ACCATGTATAATTATAAGTAGGTACCTATTTTTTTATTTTAAACTGAAATTCAATATTATATAGGCAAAG"
            "ACTTAGATGTAAGATTTCGAAGACTTGGATGTAAACAACAAATAAGATAATAACCATAAAAATAGAAATG"
            "AACGATATTAAAATTAAAAAATACGAAAAAACTAACACGTATTGTGTCCAATAAATTCGATTTGATAATT"
            "AGGTAACAATTTAACGTTAAAACCTATTCTTTTATTATCCGAAAATCCGTCGTGGAATTTGTATTAGCTT"
            "TTTTTCTACATTACCCGTTTGCGAGACAGGTGGGGTCAGACGTAGACGTAGTCTCTGGAGTCAAGACGAA"
            "ATTTTACATTTCACAATTTCCTATAGGCCGAGCAAAATTTATTAAGAACCCACAGGCATCATTACGTTTT"
            "CTTGCACAGAAGACTTCACGCTGAAGTCATTGGGCTATATTTCAACGAGACGTCTGTTGGTTTATAAAGG"
            "GCTATATTTATACAAGAATAGGAGTATGGCAGTATGCTAGGCTGGTATGTAGTACGTATACCTCCTAAGC",
            0
        );
        LOCAL (bank);

        DSK_check3_aux<32> (bank, kmerSize, nks);
        DSK_check3_aux<64> (bank, kmerSize, nks);
        DSK_check3_aux<96> (bank, kmerSize, nks);
    }
};

/********************************************************************************/

CPPUNIT_TEST_SUITE_REGISTRATION      (TestDSK);
CPPUNIT_TEST_SUITE_REGISTRATION_GATB (TestDSK);

/********************************************************************************/
} } /* end of namespaces. */
/********************************************************************************/

