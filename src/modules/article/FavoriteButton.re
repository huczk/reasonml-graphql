open ReactUtils;

[@react.component]
let make = (~id, ~count, ~slug, ~favorited, ~small=false) => {
  let {UserProvider.user} = UserProvider.useUser();

  let (favoriteMutation, _simple, _full) =
    ApolloHooks.useMutation(
      ~variables=
        Article_Query.Favorite.makeVariables(
          ~slug,
          ~favorited=!favorited,
          (),
        ),
      ~optimisticResponse=
        Gql.makeOptimisticResponse({
          "__typename": "Mutation",
          "favorite": {
            "__typename": "Article",
            "id": id,
            "favorited": !favorited,
            "favoritesCount": count->int_of_string + (favorited ? (-1) : 1),
          },
        }),
      ~refetchQueries=
        _ =>
          [|
            ApolloHooks.toQueryObj(
              Article_Query.GetArticles.make(
                ~offset=0,
                ~favorited=user->UserProvider.getUser.username,
                (),
              ),
            ),
          |],
      Article_Query.Favorite.definition,
    );

  <button
    onClick={_ =>
      switch (user) {
      | Anonymous => ReasonReactRouter.push("#/login")
      | LoggedIn(_) => favoriteMutation() |> ignore
      }
    }
    className={
      "btn btn-sm "
      ++ (favorited ? " btn-primary " : " btn-outline-primary ")
      ++ (small ? " pull-xs-right " : "")
    }>
    <i className="ion-heart" />
    " "->s
    {small ? count : "Favorite Post (" ++ count ++ ")"}->s
  </button>;
};