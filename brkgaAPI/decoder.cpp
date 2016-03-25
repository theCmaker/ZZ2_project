#include "decoder.h"
#include <algorithm>
#include "solution.h"
#include "data.h"

// sort the elements according to their keys
std::vector<size_t> makeSequence (std::vector<double> const & w)
{
  // create the Id vector
  std::vector<size_t> indices(w.size());
  std::iota(begin(indices), end(indices), static_cast<size_t>(0));

  // sort it according to the weights, uses a C++11 lambda function for the comparison
  std::sort(begin(indices), end(indices), [&](size_t i, size_t j) { return (w[i] < w[j]); });
  return indices;
}


double Decoder::decode (std::vector<double> & chromosome)
{
  std::vector<size_t> sequence = makeSequence(chromosome);
  //std::cout << "sequence" << std::endl;
  //for (unsigned i = 0; i < sequence.size(); ++i) {
  //  std::cout << sequence[i]  << " => " << data.points[sequence[i]] << std::endl;
  //}
  front.clear();

  for (size_t i = 1; i <= chromosome.size(); ++i)
  {
    Data *d = new Data(data);
    Solution solution(*d);
    d->computeConvexHull(sequence, i, solution);
	  front.add(solution);
  }

  front.computeHypervolume();

  return front.hypervolume;
}
