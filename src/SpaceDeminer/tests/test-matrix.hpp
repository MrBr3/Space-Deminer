void test_matrix()
{
  std::cout<<"==== Testing Matrix ====\n";

  check_expect(sizeof(Matrix44), 16*sizeof(gfloat));
  const Matrix44 abc( 1,  5,  9, 13,
                      2,  6, 10, 14,
                      3,  7, 11, 15,
                      4,  8, 12, 16);
  const Matrix44 i(1.f, 0.f, 0.f, 0.f,
                   0.f, 1.f, 0.f, 0.f,
                   0.f, 0.f, 1.f, 0.f,
                   0.f, 0.f, 0.f, 1.f);
  const Matrix44 s2(2.f, 0.f, 0.f, 0.f,
                    0.f, 2.f, 0.f, 0.f,
                    0.f, 0.f, 2.f, 0.f,
                    0.f, 0.f, 0.f, 1.f);
  const Vector4 c1( 1,  2,  3,  4);
  const Vector4 c2( 5,  6,  7,  8);
  const Vector4 c3( 9, 10, 11, 12);
  const Vector4 c4(13, 14, 15, 16);
  const Vector3 c4_v3(13, 14, 15);
  const Vector4 r1( 1,  5,  9, 13);
  const Vector4 r2( 2,  6, 10, 14);
  const Vector4 r3( 3,  7, 11, 15);
  const Vector4 r4( 4,  8, 12, 16);
  const Vector3 r4_v3( 4,  8, 12);

  Matrix44 m;

  SHOULD_FAIL(SHOULD_FAIL(check_within(Matrix44::identity, i)));
  m = abc;
  check_within(abc, m);
  m.set_identity();
  SHOULD_FAIL(check_within(abc, m))
  check_within(m, i);
  SHOULD_FAIL(check_within(s2, m))
  SHOULD_FAIL(check_within(m, s2))
  check_within(m.get_row(1), Vector4(1.f, 0.f, 0.f, 0.f));
  check_within(m.get_row(2), Vector4(0.f, 1.f, 0.f, 0.f));
  check_within(m.get_row(3), Vector4(0.f, 0.f, 1.f, 0.f));
  check_within(m.get_row(4), Vector4(0.f, 0.f, 0.f, 1.f));
  check_within(m.get_column(1), Vector4(1.f, 0.f, 0.f, 0.f));
  check_within(m.get_column(2), Vector4(0.f, 1.f, 0.f, 0.f));
  check_within(m.get_column(3), Vector4(0.f, 0.f, 1.f, 0.f));
  check_within(m.get_column(4), Vector4(0.f, 0.f, 0.f, 1.f));
  check_within(abc.get_row(1), r1);
  check_within(abc.get_row(2), r2);
  check_within(abc.get_row(3), r3);
  check_within(abc.get_row(4), r4);
  check_within(abc.get_column(1), c1);
  check_within(abc.get_column(2), c2);
  check_within(abc.get_column(3), c3);
  check_within(abc.get_column(4), c4);
  check_within(abc, Matrix44(c1, c2, c3, c4));
  check_within(abc, Matrix44(c1, c2, c3, c4_v3, 16.f));

  m.set( 1,  5,  9, 13,
         2,  6, 10, 14,
         3,  7, 11, 15,
         4,  8, 12, 16);

  check_within(abc, m);
  m.set_identity();
  SHOULD_FAIL(check_within(abc, m))

  m.set_by_columns(c1, c2, c3, c4);

  check_within(abc, m);
  m.set_identity();
  SHOULD_FAIL(check_within(abc, m))

  m.set_by_columns(c1, c2, c3, c4_v3, 16.f);

  check_within(abc, m);
  m.set_identity();
  SHOULD_FAIL(check_within(abc, m))

  m.set_by_rows(r1, r2, r3, r4);

  check_within(abc, m);
  m.set_identity();
  SHOULD_FAIL(check_within(abc, m))

  m.set_by_rows(r1, r2, r3, r4_v3, 16.f);

  check_within(abc, m);
  m.set_identity();
  SHOULD_FAIL(check_within(abc, m))

  m.set_column(1, c1);
  m.set_column(2, c2);
  m.set_column(3, c3);
  m.set_column(4, c4);
  SHOULD_FAIL(m.set_column(0, r4))
  SHOULD_FAIL(m.set_column(5, r4))
  SHOULD_FAIL(m.set_column(6, r4))

  check_within(abc, m);
  m.set_identity();
  SHOULD_FAIL(check_within(abc, m))

  m.set_row(1, r1);
  m.set_row(2, r2);
  m.set_row(3, r3);
  m.set_row(4, r4);
  SHOULD_FAIL(m.set_row(0, r4))
  SHOULD_FAIL(m.set_row(5, r4))
  SHOULD_FAIL(m.set_row(6, r4))

  check_within(abc, m);
  m.set_identity();
  SHOULD_FAIL(check_within(abc, m))

  m(1, 1) = 1.f;
  m(2, 1) = 2.f;
  m(3, 1) = 3.f;
  m(4, 1) = 4.f;

  m(1, 2) = 5.f;
  m(2, 2) = 6.f;
  m(3, 2) = 7.f;
  m(4, 2) = 8.f;

  m(1, 3) = 9.f;
  m(2, 3) = 10.f;
  m(3, 3) = 11.f;
  m(4, 3) = 12.f;

  m(1, 4) = 13.f;
  m(2, 4) = 14.f;
  m(3, 4) = 15.f;
  m(4, 4) = 16.f;
  SHOULD_FAIL(m(0, 0))
  SHOULD_FAIL(m(0, 1))
  SHOULD_FAIL(m(1, 0))
  SHOULD_FAIL(m(1, 5))
  SHOULD_FAIL(m(5, 5))
  SHOULD_FAIL(m(5, 1))

  for(gsize r = 1; r<=4; ++r)
  for(gsize c = 1; c<=4; ++c)
  {
    check_within(m(r, c) , abc(r, c));
  }

  check_within(abc, m);
  check_within(m(1, 1), 1.f);
  SHOULD_FAIL(check_within(m(2, 1), 1.f));
  check_within(m(2, 1), 2.f);
  check_within(m(3, 1), 3.f);
  check_within(m(4, 1), 4.f);
  check_within(m(1, 2), 5.f);
  check_within(m(2, 2), 6.f);
  check_within(m(3, 2), 7.f);
  check_within(m(4, 2), 8.f);
  check_within(m(1, 3), 9.f);
  check_within(m(2, 3), 10.f);
  check_within(m(3, 3), 11.f);
  check_within(m(4, 3), 12.f);
  check_within(m(1, 4), 13.f);
  check_within(m(2, 4), 14.f);
  check_within(m(3, 4), 15.f);
  check_within(m(4, 4), 16.f);
  m.set_identity();
  SHOULD_FAIL(check_within(abc, m))

  m.set_scale(2.f);
  check_within(m, s2);
  m.set_identity();
  SHOULD_FAIL(check_within(s2, m))

  m.set_scale(23.f, 42.f, 64.f);

  check_within(m, Matrix44(23.f,  0.f,  0.f,  0.f,
                            0.f, 42.f,  0.f,  0.f,
                            0.f,  0.f, 64.f,  0.f,
                            0.f,  0.f,  0.f,  1.f));
  m.set_identity();

  m.scale(2.f);
  check_within(m, s2);
  m.scale(0.5f);
  check_within(m, i);
  m.scale(8.f);
  check_within(m, Matrix44(s2).scale(4.f));
  m.set_identity();

  m.rotate_z(42.f);
  m.rotate_z(-42.f);

  check_within(m, Matrix44(1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 1.f, 0.f,
                           0.f, 0.f, 0.f, 1.f));
  m.set_identity();

  m.rotate_z(90.f);
  m.rotate_z(270.f);

  check_within(m, Matrix44(1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 1.f, 0.f,
                           0.f, 0.f, 0.f, 1.f));
  m.set_identity();

  m.rotate_z(90.f);

  check_within(m, Matrix44(0.f,-1.f, 0.f, 0.f,
                           1.f, 0.f, 0.f, 0.f,
                           0.f, 0.f, 1.f, 0.f,
                           0.f, 0.f, 0.f, 1.f));

  m.set_identity();

  m.rotate_z(180.f);

  check_within(m, Matrix44(-1.f, 0.f, 0.f, 0.f,
                            0.f,-1.f, 0.f, 0.f,
                            0.f, 0.f, 1.f, 0.f,
                            0.f, 0.f, 0.f, 1.f));
  m.set_identity();

  m.rotate_x(42.f);
  m.rotate_x(-42.f);

  check_within(m, Matrix44(1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 1.f, 0.f,
                           0.f, 0.f, 0.f, 1.f));
  m.set_identity();

  m.rotate_x(92.f);
  m.rotate_x(268.f);

  check_within(m, Matrix44(1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 1.f, 0.f,
                           0.f, 0.f, 0.f, 1.f));
  m.set_identity();

  m.rotate_x(90.f);

  check_within(m, Matrix44(1.f, 0.f, 0.f, 0.f,
                           0.f, 0.f,-1.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 0.f, 1.f));

  m.set_identity();

  m.rotate_x(180.f);

  check_within(m, Matrix44( 1.f, 0.f, 0.f, 0.f,
                            0.f,-1.f, 0.f, 0.f,
                            0.f, 0.f,-1.f, 0.f,
                            0.f, 0.f, 0.f, 1.f));
  m.set_identity();

  m.rotate_y(42.f);
  m.rotate_y(-42.f);

  check_within(m, Matrix44(1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 1.f, 0.f,
                           0.f, 0.f, 0.f, 1.f));
  m.set_identity();

  m.rotate_y(92.f);
  m.rotate_y(268.f);

  check_within(m, Matrix44(1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 1.f, 0.f,
                           0.f, 0.f, 0.f, 1.f));
  m.set_identity();

  m.rotate_y(90.f);

  check_within(m, Matrix44( 0.f, 0.f, 1.f, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                           -1.f, 0.f, 0.f, 0.f,
                            0.f, 0.f, 0.f, 1.f));

  m.set_identity();

  m.rotate_y(180.f);

  check_within(m, Matrix44(-1.f, 0.f, 0.f, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                            0.f, 0.f,-1.f, 0.f,
                            0.f, 0.f, 0.f, 1.f));
  m.set_identity();

  m.translate( 42.f, 32.f,-23.f);
  m.translate(-42.f,-32.f, 23.f);

  check_within(m, Matrix44(1.f, 0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f, 0.f,
                           0.f, 0.f, 1.f, 0.f,
                           0.f, 0.f, 0.f, 1.f));
  m.set_identity();

  m.translate( 42.f, 32.f,-23.f);
  m.translate(  1.f, -2.f,-3.f);

  check_within(m, Matrix44(1.f, 0.f, 0.f, 43.f,
                           0.f, 1.f, 0.f, 30.f,
                           0.f, 0.f, 1.f,-26.f,
                           0.f, 0.f, 0.f,  1.f));
  m.set_identity();

  m.translate(5.f, 7.f, 11.f);

  check_within(m, Matrix44(1.f, 0.f, 0.f,  5.f,
                           0.f, 1.f, 0.f,  7.f,
                           0.f, 0.f, 1.f, 11.f,
                           0.f, 0.f, 0.f,  1.f));

  m.set_identity();

  m.translate(0.f, 0.f, 0.f);

  check_within(m, Matrix44( 1.f, 0.f, 0.f, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                            0.f, 0.f, 1.f, 0.f,
                            0.f, 0.f, 0.f, 1.f));

  SHOULD_FAIL(m.set_row(0, Vector4(42.f, 42.f, 42.f, 42.f)));
  SHOULD_FAIL(m.set_row(5, Vector4(42.f, 42.f, 42.f, 42.f)));
  SHOULD_FAIL(m.set_column(0, Vector4(42.f, 42.f, 42.f, 42.f)));
  SHOULD_FAIL(m.set_column(5, Vector4(42.f, 42.f, 42.f, 42.f)));
  SHOULD_FAIL(m.get_row(0));
  SHOULD_FAIL(m.get_row(5));
  SHOULD_FAIL(m.get_column(0));
  SHOULD_FAIL(m.get_column(5));

  check_within(m, Matrix44( 1.f, 0.f, 0.f, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                            0.f, 0.f, 1.f, 0.f,
                            0.f, 0.f, 0.f, 1.f));

  SHOULD_FAIL(abc.get_single_item_of_sub_3x3_matrix(4, 5, 1, 1))
  SHOULD_FAIL(abc.get_single_item_of_sub_3x3_matrix(4, 6, 1, 1))
  SHOULD_FAIL(abc.get_single_item_of_sub_3x3_matrix(5, 4, 1, 1))
  SHOULD_FAIL(abc.get_single_item_of_sub_3x3_matrix(2, 0, 1, 1))
  SHOULD_FAIL(abc.get_single_item_of_sub_3x3_matrix(0, 2, 1, 1))
  SHOULD_FAIL(abc.get_single_item_of_sub_3x3_matrix(2, 2, 0, 1))
  SHOULD_FAIL(abc.get_single_item_of_sub_3x3_matrix(2, 2, 1, 0))
  SHOULD_FAIL(abc.get_single_item_of_sub_3x3_matrix(2, 2, 1, 5))
  SHOULD_FAIL(abc.get_single_item_of_sub_3x3_matrix(2, 2, 5, 5))

  check_within<gfloat>(abc.get_single_item_of_sub_3x3_matrix(1, 3, 4, 3), 13.);

  SHOULD_FAIL(check_expect<std::string>(abc.str_sub3x3(4, 4), "(1  5  9)\n"
                                                              "(2  6  10)\n"
                                                              "(3  7  11)\n"))
  check_expect<std::string>(abc.str_sub3x3(4, 4), "(1  5  9)\n"
                                                  "(2  6  10)\n"
                                                  "(3  7  11)");
  check_expect<std::string>(abc.str_sub3x3(4, 3), "(1  5  13)\n"
                                                  "(2  6  14)\n"
                                                  "(3  7  15)");
  check_expect<std::string>(abc.str_sub3x3(4, 2), "(1  9  13)\n"
                                                  "(2  10  14)\n"
                                                  "(3  11  15)");
  check_expect<std::string>(abc.str_sub3x3(4, 1), "(5  9  13)\n"
                                                  "(6  10  14)\n"
                                                  "(7  11  15)");
  check_expect<std::string>(abc.str_sub3x3(3, 4), "(1  5  9)\n"
                                                  "(2  6  10)\n"
                                                  "(4  8  12)");
  check_expect<std::string>(abc.str_sub3x3(3, 3), "(1  5  13)\n"
                                                  "(2  6  14)\n"
                                                  "(4  8  16)");
  check_expect<std::string>(abc.str_sub3x3(3, 2), "(1  9  13)\n"
                                                  "(2  10  14)\n"
                                                  "(4  12  16)");
  check_expect<std::string>(abc.str_sub3x3(3, 1), "(5  9  13)\n"
                                                  "(6  10  14)\n"
                                                  "(8  12  16)");
  check_expect<std::string>(abc.str_sub3x3(2, 4), "(1  5  9)\n"
                                                  "(3  7  11)\n"
                                                  "(4  8  12)");
  check_expect<std::string>(abc.str_sub3x3(2, 3), "(1  5  13)\n"
                                                  "(3  7  15)\n"
                                                  "(4  8  16)");
  check_expect<std::string>(abc.str_sub3x3(2, 2), "(1  9  13)\n"
                                                  "(3  11  15)\n"
                                                  "(4  12  16)");
  check_expect<std::string>(abc.str_sub3x3(2, 1), "(5  9  13)\n"
                                                  "(7  11  15)\n"
                                                  "(8  12  16)");
  check_expect<std::string>(abc.str_sub3x3(1, 4), "(2  6  10)\n"
                                                  "(3  7  11)\n"
                                                  "(4  8  12)");
  check_expect<std::string>(abc.str_sub3x3(1, 3), "(2  6  14)\n"
                                                  "(3  7  15)\n"
                                                  "(4  8  16)");
  check_expect<std::string>(abc.str_sub3x3(1, 2), "(2  10  14)\n"
                                                  "(3  11  15)\n"
                                                  "(4  12  16)");
  check_expect<std::string>(abc.str_sub3x3(1, 1), "(6  10  14)\n"
                                                  "(7  11  15)\n"
                                                  "(8  12  16)");

  m = abc;
  m.transpose();

  check_within(m, Matrix44(1.f,  2.f,  3.f,  4.f,
                           5.f,  6.f,  7.f,  8.f,
                           9.f, 10.f, 11.f, 12.f,
                           13.f, 14.f, 15.f, 16.f));
  check_within(m, abc.get_transposed());
  m.transpose();
  check_within(m, abc);
  check_within(abc.get_transposed().get_transposed(), abc);
  m.set_identity();
}