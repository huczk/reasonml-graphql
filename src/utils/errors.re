open ReactUtils;

let getGqlErrors =
    (simpleResult: ApolloHooks.Mutation.controlledVariantResult('a)) =>
  switch (simpleResult) {
  | Error(error) =>
    switch (error##graphQLErrors |> Js.Nullable.toOption) {
    | None => React.null
    | Some(errors) =>
      errors
      ->Belt.Array.map(err => <li key=err##message> {err##message->s} </li>)
      ->React.array
    }

  | _ => React.null
  };

let getFormErrors = (fieldsState: array((_, BsReform.ReForm.fieldState))) =>
  fieldsState
  ->Belt.Array.keepMap(
      fun
      | (_, Error(error)) => Some(<li key=error> error->s </li>)
      | _ => None,
    )
  ->React.array;