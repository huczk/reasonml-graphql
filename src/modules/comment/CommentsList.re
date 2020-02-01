[@react.component]
let make = (~slug) => {
  let (simple, _full) =
    ApolloHooks.useQuery(
      ~variables=Comment_Query.GetComments.makeVariables(~slug, ()),
      Comment_Query.GetComments.definition,
    );

  <>
    {switch (simple) {
     | Data(data) =>
       data##comments
       ->Belt.Array.keepMap(
           fun
           | None => None
           | Some(comment) => Some(<Comment key=comment##id comment slug />),
         )
       |> React.array
     | _ => React.null
     }}
  </>;
};