type loggedUser = User_Query.t;

type user =
  | Anonymous
  | LoggedIn(loggedUser);

type ctx = {
  user,
  login: option(string) => unit,
  logout: unit => unit,
};

module Context = {
  let context =
    React.createContext({user: Anonymous, login: ignore, logout: ignore});

  module Provider = {
    let make = context->React.Context.provider;

    let makeProps = (~value, ~children, ()) => {
      "value": value,
      "children": children,
    };
  };
};

let useUser = () => React.useContext(Context.context);

let isLoggedUser = (user, id) =>
  switch (user) {
  | LoggedIn(user) => compare(user.id, id) == 0
  | Anonymous => false
  };

let getUser =
  fun
  | LoggedIn(user) => user
  | Anonymous => {
      id: "",
      username: "",
      email: "",
      image: "",
      bio: "",
      token: None,
    };

[@react.component]
let make = (~children) => {
  let (_simple, full) =
    ApolloHooks.useQuery(
      ~errorPolicy=Ignore,
      ~fetchPolicy=NetworkOnly,
      User_Query.User.definition,
    );

  let user =
    switch (full) {
    | {data: Some(data)} =>
      switch (data##user) {
      | Some(user) => LoggedIn(user)
      | None => Anonymous
      }
    | _ => Anonymous
    };

  let value =
    React.useMemo2(
      () =>
        {
          user,
          login: token => {
            Effects.saveTokenToStorage(token);
            full.refetch() |> ignore;
          },
          logout: () => {
            Effects.removeTokenFromStorage();
            Client.client##resetStore()
            |> Js.Promise.then_(() => {
                 ReasonReactRouter.push("/");
                 Js.Promise.resolve();
               })
            |> ignore;
          },
        },
      (full.refetch, user),
    );

  <Context.Provider value> children </Context.Provider>;
};