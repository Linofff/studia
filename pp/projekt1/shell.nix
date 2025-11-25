{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  buildInputs = [ pkgs.ncurses pkgs.gcc pkgs.cmake pkgs.clang-tools];
}
