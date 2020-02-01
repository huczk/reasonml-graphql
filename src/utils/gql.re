external makeOptimisticResponse: 'a => Js.Json.t = "%identity";

let randomId = () => Random.int(1000) |> string_of_int;

let decodeDate = date =>
  date
  ->Js.Json.decodeString
  ->Belt.Option.getWithDefault("")
  ->Js.Date.fromString
  ->Js.Date.toDateString;

let encodeDate = (~date=Js.Date.make(), ()) => date->Js.Date.toISOString;