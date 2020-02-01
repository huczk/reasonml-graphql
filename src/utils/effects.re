let tokenKey = "jwt";

let saveTokenToStorage =
  fun
  | Some(value) => Dom.Storage.(localStorage |> setItem(tokenKey, value))
  | None => ();

let getTokenFromStorage = () =>
  Dom.Storage.(localStorage |> getItem(tokenKey));

let removeTokenFromStorage = () =>
  Dom.Storage.(localStorage |> removeItem(tokenKey));