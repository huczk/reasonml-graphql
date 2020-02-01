type t =
  | Home
  | Login
  | Register
  | Settings
  | CreateArticle
  | EditArticle(string)
  | Article(string)
  | Profile(string, bool);

let useRoute = () => {
  let {ReasonReactRouter.hash} = ReasonReactRouter.useUrl();
  let hash = hash |> Js.String.split("/");

  switch (hash) {
  | [|"", "login"|] => Login
  | [|"", "register"|] => Register
  | [|"", "settings"|] => Settings
  | [|"", "editor"|] => CreateArticle
  | [|"", "editor", slug|] => EditArticle(slug)
  | [|"", "article", slug|] => Article(slug)
  | [|"", "profile", username|] => Profile(username, false)
  | [|"", "profile", username, "favorites"|] => Profile(username, true)
  | _ => Home
  };
};