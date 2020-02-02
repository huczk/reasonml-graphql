open Jest;
open Expect;
open DomTestingLibrary;
open ReactTestingLibrary;

[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

type tagsData = {tags: array(string)};

let mocks: array(MockedProvider.mock(tagsData)) = [|
  {
    request: {
      query: gql(. Tag_Query.GetTags.query),
    },
    result: {
      data: {
        tags: [|"Hello"|],
      },
    },
  },
|];

test("Component renders when loading", () =>
  <MockedProvider mocks=[||]> <TagsPopular /> </MockedProvider>
  |> render
  |> container
  |> expect
  |> toMatchSnapshot
);

testPromise("Component renders resolved data", () => {
  let container =
    <MockedProvider mocks> <TagsPopular /> </MockedProvider>
    |> render
    |> container;

  wait()
  |> Js.Promise.then_(_ => {
       Js.Promise.resolve(container |> expect |> toMatchSnapshot)
     });
});