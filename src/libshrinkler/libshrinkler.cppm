// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

export module libshrinkler;

namespace libshrinkler
{

inline constexpr int min_preset = 1;
inline constexpr int max_preset = 9;
inline constexpr int default_preset = 3;

inline constexpr int min_references = 1000;
inline constexpr int max_references = 100000000;
inline constexpr int default_references = 100000;

inline constexpr int min_iterations = 1;
inline constexpr int max_iterations = 9;

inline constexpr int min_length_margin = 0;
inline constexpr int max_length_margin = 100;

inline constexpr int min_same_length = 1;
inline constexpr int max_same_length = 100000;

inline constexpr int min_effort = 0;
inline constexpr int max_effort = 100000;

inline constexpr int min_skip_length = 2;
inline constexpr int max_skip_length = 100000;

export class compression_parameters final
{
public:
    explicit compression_parameters(int preset = default_preset);

    void preset(int preset);

    bool verbose() const { return m_verbose; }

    void verbose(bool verbose);

    bool parity_context() const { return m_parity_context; }

    void parity_context(bool parity_context);

    int references() const { return m_references; }

    void references(int references);

    int iterations() const { return m_iterations; }

    void iterations(int iterations);

    int length_margin() const { return m_length_margin; }

    void length_margin(int length_margin);

    int same_length() const { return m_same_length; }

    void same_length(int same_length);

    int effort() const { return m_effort; }

    void effort(int effort);

    int skip_length() const { return m_skip_length; }

    void skip_length(int skip_length);

private:
    bool m_verbose = false;
    bool m_parity_context = true;
    int m_references = default_references;
    int m_iterations;
    int m_length_margin;
    int m_same_length;
    int m_effort;
    int m_skip_length;
};

}
