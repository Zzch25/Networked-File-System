# CMake generated Testfile for 
# Source directory: /mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests
# Build directory: /mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(test_hostkey "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests/test_hostkey")
SET_TESTS_PROPERTIES(test_hostkey PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests")
ADD_TEST(test_hostkey_hash "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests/test_hostkey_hash")
SET_TESTS_PROPERTIES(test_hostkey_hash PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests")
ADD_TEST(test_password_auth_succeeds_with_correct_credentials "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests/test_password_auth_succeeds_with_correct_credentials")
SET_TESTS_PROPERTIES(test_password_auth_succeeds_with_correct_credentials PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests")
ADD_TEST(test_password_auth_fails_with_wrong_password "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests/test_password_auth_fails_with_wrong_password")
SET_TESTS_PROPERTIES(test_password_auth_fails_with_wrong_password PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests")
ADD_TEST(test_password_auth_fails_with_wrong_username "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests/test_password_auth_fails_with_wrong_username")
SET_TESTS_PROPERTIES(test_password_auth_fails_with_wrong_username PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests")
ADD_TEST(test_public_key_auth_fails_with_wrong_key "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests/test_public_key_auth_fails_with_wrong_key")
SET_TESTS_PROPERTIES(test_public_key_auth_fails_with_wrong_key PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests")
ADD_TEST(test_public_key_auth_succeeds_with_correct_rsa_key "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests/test_public_key_auth_succeeds_with_correct_rsa_key")
SET_TESTS_PROPERTIES(test_public_key_auth_succeeds_with_correct_rsa_key PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests")
ADD_TEST(test_public_key_auth_succeeds_with_correct_dsa_key "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests/test_public_key_auth_succeeds_with_correct_dsa_key")
SET_TESTS_PROPERTIES(test_public_key_auth_succeeds_with_correct_dsa_key PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests")
ADD_TEST(test_keyboard_interactive_auth_fails_with_wrong_response "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests/test_keyboard_interactive_auth_fails_with_wrong_response")
SET_TESTS_PROPERTIES(test_keyboard_interactive_auth_fails_with_wrong_response PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests")
ADD_TEST(test_keyboard_interactive_auth_succeeds_with_correct_response "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/bin/tests/test_keyboard_interactive_auth_succeeds_with_correct_response")
SET_TESTS_PROPERTIES(test_keyboard_interactive_auth_succeeds_with_correct_response PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests")
ADD_TEST(mansyntax "/bin/sh" "-c" "srcdir=/mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests /mnt/c/Users/JOBZ/Desktop/Work/GAPrac/EXER/CPP_PRAC/MICRO_PROJECTS/NETWORKING_SERIES/SRC_FS_MERGE/EXT_LIBS/libssh2/tests/mansyntax.sh")
