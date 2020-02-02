open Jest;
open Expect;
open ReactTestingLibrary;

test("Component renders", () =>
  <Footer /> |> render |> container |> expect |> toMatchSnapshot
);