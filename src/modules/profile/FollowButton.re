open ReactUtils;

[@react.component]
let make = (~id, ~username, ~following, ~className="") => {
  let {UserProvider.user} = UserProvider.useUser();
  let (followMutation, _simple, _full) =
    ApolloHooks.useMutation(
      ~variables=
        Profile_Query.Follow.makeVariables(~username, ~follow=!following, ()),
      ~optimisticResponse=
        Gql.makeOptimisticResponse({
          "__typename": "Mutation",
          "follow": {
            "__typename": "Profile",
            "id": id,
            "following": !following,
          },
        }),
      ~refetchQueries=
        _ =>
          [|
            ApolloHooks.toQueryObj(
              Article_Query.GetFeed.make(~offset=0, ()),
            ),
          |],
      Profile_Query.Follow.definition,
    );

  <button
    onClick={_ =>
      switch (user) {
      | Anonymous => ReasonReactRouter.push("#/login")
      | LoggedIn(_) => followMutation() |> ignore
      }
    }
    className={
      "btn btn-sm "
      ++ (following ? " btn-secondary " : " btn-outline-secondary ")
      ++ className
    }>
    <i className="ion-plus-round" />
    {{
       (following ? " Unfollow " : " Follow ") ++ username;
     }
     ->s}
  </button>;
};