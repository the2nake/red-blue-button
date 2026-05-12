#include <algorithm>
#include <print>
#include <random>

// these don't affect the outcome
const int trials = 100;        // number of experiments (virtual worlds)
const int min_gen = 1000;      // min times to do the problem per world
const int actor_count = 1000;  // number of people in the world

// importance of self-survival
// 0.01 = self-survival is as important as 2% of the population surviving
// i.e. this is pretty selfish
// simulation bifurcates between selfishness = 0.01 and 0.015
const float selfishness = 0.01;

// how much to adjust the weights. did not affect outcome
const float learn_rate = 10.f / min_gen;

// mean of initial weights. bias strongly dictates outcome
const float initial_mean = 0.5;

// range of initial weights. did not affect bias outcome
const float initial_range = 0.5;

std::random_device r;
std::seed_seq ss{r(), r(), r(), r(), r(), r(), r(), r(), r()};
std::mt19937 e(ss);
// dist is a uniform rng from 0.0 to 1.0
std::uniform_real_distribution<> dist;

// returns final weight (inclination to pick blue/red)
// and also final survival rate
// weights are: blue 0.0 --> 1.0 red
std::pair<float, float> run_experiment(bool print, int every) {
  // pairs of {probability (float), pick (bool)}
  // probability: 0.0 (blue) --> 1.0 (red)
  // pick:        0 (blue), 1 (red)
  std::vector<std::pair<float, bool>> actors(actor_count);
  // randomise initial to the [mean - range / 2, mean + range / 2]
  for (auto& [weight, choice] : actors) {
    weight = initial_mean + (dist(e) - 0.5) * initial_range;
  }
  float survivors;

  int unc = 1;
  // experiment ends once every no more actors change their choices
  for (int iter = 0; iter < min_gen || unc; ++iter) {
    // track how many actors are still undecided
    unc = 0;
    for (auto& [w, _] : actors) { unc += (0.05 < w && w <= 0.95); }
    if (unc == 0) break;

    // generate choices
    int blue = 0;
    for (auto& [weight, choice] : actors) {
      choice = dist(e) <= weight;  // lower p, less likely to pick blue
      blue += !choice;
    };

    // count proportion of survivors
    survivors = 1.f;
    if (blue < actor_count / 2) { survivors -= (float)blue / actor_count; }

    // adjust weights
    for (auto& [weight, choice] : actors) {
      float initial_dir = (choice ? 1 : -1);
      // confidence inverts the direction if less than half the people survived
      float confidence = (1 - selfishness) * (survivors - 0.5);
      confidence += selfishness * (!choice && survivors != 1.f ? -1.0 : 1.0);

      float correction = initial_dir * confidence;
      weight += correction * learn_rate;
      weight = std::clamp(weight, 0.f, 1.f);
    }

    if (print && iter % every == 0) {
      if (iter <= min_gen) {
        std::print("{:.2f} ", survivors);
      } else {
        std::print(".");
      }
    }
  }

  float avg = 0.f;
  for (auto& [p, _] : actors) { avg += p; }
  avg /= actors.size();

  if (print) { std::print("| w = {:.3f}", avg); }
  return {avg, survivors};
}

int main() {
  bool print_trials = true;

  std::vector<std::pair<float, float>> results;
  int red_conv = 0;

  for (int i = 0; i < trials; ++i) {
    print_trials = !(i % 10);
    if (print_trials) { std::print("exp {: 3} survival | ", i); }
    auto res = run_experiment(print_trials, min_gen / 5);
    results.emplace_back(res);
    if (print_trials) { std::println(); }

    // special consideration to show how rare red is
    if (res.first > 0.5) { ++red_conv; }
  }

  float avg_w = 0.f, stdev_w = 0.f;
  float avg_bs = 0.f, stdev_bs = 0.f;
  float avg_rs = 0.f, stdev_rs = 0.f;
  int nb = 0, nr = 0;
  for (const auto& [w, s] : results) {
    avg_w += w;
    (w <= 0.5 ? avg_bs : avg_rs) += s;
    ++(w <= 0.5 ? nb : nr);
  }
  avg_w /= results.size();
  avg_bs /= nb;
  avg_rs /= nr;
  for (const auto& [w, s] : results) {
    stdev_w += (avg_w - w) * (avg_w - w);
    float dev_s = (w <= 0.5 ? avg_bs : avg_rs) - s;
    (w <= 0.5 ? stdev_bs : stdev_rs) += dev_s * dev_s;
  }
  stdev_w = std::sqrt(stdev_w / (results.size() - 1));
  stdev_bs = std::sqrt(stdev_bs / (nb - 1));
  stdev_rs = std::sqrt(stdev_rs / (nr - 1));

  std::println("---");
  std::println("  avg (weight): {:.3f} (blue 0.0 -> 1.0 red)", avg_w);
  std::println("stdev (weight): {:.3f}", stdev_w);
  std::println("{} / {} worlds led to red (w > 0.5)", red_conv, results.size());
  std::println();
  std::println("  avg (blue survival): {:.3f}", avg_bs);
  std::println("stdev (blue survival): {:.3f}", stdev_bs);
  std::println("  avg (red survival): {:.3f}", avg_rs);
  std::println("stdev (red survival): {:.3f}", stdev_rs);

  // note: average weight skews towards red because converging on red requires
  // stronger confidence in the red choice
}
