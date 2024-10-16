//
// Created by Obi Davis on 13/09/2024.
//

#include "generate.hpp"
#include "solve.hpp"
#include "output.hpp"
#include "CLI/App.hpp"

int main(int argc, char **argv) {
    CLI::App app("Sudoku CLI utility", "singledigit" );

    setup_generate(app);
    setup_solve(app);
    app.require_subcommand(1);

    app.set_help_flag("");
    app.set_help_all_flag("-h, --help");

    try {
        app.parse(argc, argv);
        return 0;
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return 1;
    }
}
