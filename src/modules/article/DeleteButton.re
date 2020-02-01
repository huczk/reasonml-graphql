open ReactUtils;

let delete = deleteMutation => {
  deleteMutation()
  |> Js.Promise.then_(
       (result: ApolloHooks.Mutation.result(Article_Query.Delete.t)) => {
       let success =
         switch (result) {
         | Data(data) => data##deleteArticle##success
         | _ => false
         };

       if (success) {
         ReasonReactRouter.push("/");
       };

       Js.Promise.resolve();
     })
  |> ignore;
};

[@react.component]
let make = (~slug) => {
  let (mutation, _simple, _full) =
    ApolloHooks.useMutation(
      ~variables=Article_Query.Delete.makeVariables(~slug, ()),
      Article_Query.Delete.definition,
    );

  <button
    onClick={_ => delete(mutation)} className="btn btn-outline-danger btn-sm">
    <i className="ion-trash-a" />
    " Delete Article"->s
  </button>;
};