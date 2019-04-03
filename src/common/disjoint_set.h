// Copyright (c) 2017, The Johns Hopkins University /
// Applied Physics Laboratory (JHU/APL)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Please reference the following when reporting any results using this software:
//
// M. Bosch, A. Leichtman, D. Chilcott, H. Goldberg, M. Brown, “Metric
// Evaluation Pipeline for 3D Modeling of Urban Scenes,” ISPRS Archives, 2017.
//
// S. Almes, S. Hagstrom, D. Chilcott, H. Goldberg, M. Brown, “Open Source
// Geospatial Tools to Enable Large Scale 3D Scene Modeling,” FOSS4G, 2017.
//
// For more information, please see: http://www.jhuapl.edu/pubgeo.html

#ifndef PUBGEO_DISJOINT_SET_H_
#define PUBGEO_DISJOINT_SET_H_

#include <vector>

namespace pubgeo {

/**
 * Implements a disjoint-set data structure
 * (see https://en.wikipedia.org/wiki/Disjoint-set_data_structure)
 *
 * Class is designed to manage a series of sets, which can be merged together
 *
 * Example usage:
 *
 * add();   // Adds 0, Collection contains:     [0]
 * add();   // Adds 1, Collection contains:     [0, 1]
 * add();   // Adds 2, Collection contains:     [0, 1, 2]
 * add();   // Adds 3, Collection contains:     [0, 1, 2, 3]
 * find(3); // Returns 3, collection contains:  [0, 1, 2, 3]
 * merge(2,3); // Merges 2 & 3, collection is:  [0, 1, 2, 2]
 * find(3); // Returns 2, collection contains:  [0, 1, 2, 2]
 * merge(1,2); // Merges 1 & 2, collection is:  [0, 1, 1, 2]
 * merge(0,2); // Merges 0 & 2, collection is:  [0, 0, 1, 2]
 * find(2); // Returns 0, collection now is:    [0, 0, 0, 2] // Path compression updates 2's label
 * find(3); // Returns 0, collection now is:    [0, 0, 0, 0] // Path compression updates 3's label
 */
class DisjointSet : public std::vector<size_t> {
public:

    /**
     * Add a new set to the collection
     *
     * Returns the label of the added set
     */
    size_t add() {
        size_t n = size();
        push_back(n);
        return n;
    }

    /**
     * Find the label of the merged-set that the given label belongs to
     */
    size_t find(size_t lbl) {
        if (this->at(lbl) == lbl) {
            return lbl;
        } else {
            size_t& root = this->at(lbl);
            root = find(root); // Path compression
            return root;
        }
    }

    /**
     * Merge two sets together
     */
    void merge(size_t x, size_t y) {
        if (x == y)
            return;

        size_t a = find(x);
        size_t b = find(y);

        if (a < b)
            this->at(b) = a;
        else if (b < a)
            this->at(a) = b;
    }


    /**
     * Convenience function that re-maps set labels to a consecutive range
     *
     * Labels can be "lost" as sets are merged (e.g. if out of sets [0, 1, 2, 3],
     * 1 & 2 are merged, the resulting set labels would be [0, 1, 1, 3]); this provides
     * a lookup table that would map set labels to a continuous range starting at
     * MIN_LABEL.  So for the example, if MIN_LABEL is 1, the returned lookup table
     * would be [1, 2, 2, 3].  This table is invalidated upon a merge operation.
     */
    template<class T = size_t>
    vector<T> flatten(T min_label = 0) {
        vector<T> out(size());
        for (size_t i = 0; i<size(); ++i) {
            size_t j = find(i);
            out[i] = (j==i) ? min_label++ : out[j]; // Deliberately using post-increment
        }
        return out;
    }
};

} // pubgeo

#endif /* PUBGEO_DISJOINT_SET_H_ */
