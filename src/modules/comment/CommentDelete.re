open ReactUtils;

[@react.component]
let make = (~id, ~slug) => {
  let (mutation, _simple, _full) =
    ApolloHooks.useMutation(
      ~variables=Comment_Query.CommentDelete.makeVariables(~id, ()),
      ~optimisticResponse=
        Gql.makeOptimisticResponse({
          "__typename": "Mutation",
          "deleteComment": {
            "__typename": "Comment",
            "id": id,
          },
        }),
      ~update=
        (client, _) =>
          Comment_Query.update(
            ~client,
            ~slug,
            ~updateData=Comment_Query.removeFromCache(~id),
          ),
      Comment_Query.CommentDelete.definition,
    );

  <i
    className="ion-trash-a"
    onClick={eventPrevent(_ => mutation() |> ignore)}
  />;
};