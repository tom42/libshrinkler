// SPDX-FileCopyrightText: 2025 Thomas Mathys
// SPDX-License-Identifier: MIT

export module libshrinkler;

namespace libshrinkler
{

export inline constexpr int default_preset = 3;

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

    int effort() const { return m_effort; }

    int skip_length() const { return m_skip_length; }

    // TODO: once it builds this needs a test
    // TODO: add setters for all fields (range checks?(
private:
    bool m_verbose = false;
    bool m_parity_context = true;
    int m_references = 100000; // TODO: constant?
    int m_iterations;
    int m_length_margin;
    int m_same_length;
    int m_effort;
    int m_skip_length;
};

// TODO: here are min/max values:
/*
IntParameter    iterations    ("-i", "--iterations",      1,        9,    1*p, argc, argv, consumed);
IntParameter    length_margin ("-l", "--length-margin",   0,      100,    1*p, argc, argv, consumed);
IntParameter    same_length   ("-a", "--same-length",     1,   100000,   10*p, argc, argv, consumed);
IntParameter    effort        ("-e", "--effort",          0,   100000,  100*p, argc, argv, consumed);
IntParameter    skip_length   ("-s", "--skip-length",     2,   100000, 1000*p, argc, argv, consumed);
IntParameter    references    ("-r", "--references",   1000,100000000, 100000, argc, argv, consumed);
*/

}
