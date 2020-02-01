open ReactUtils;

[@react.component]
let make = (~tagList) => {
  <ul className="tag-list">
    {tagList->Belt.Array.keepMap(
       fun
       | None => None
       | Some(tag) =>
         Some(
           <li key=tag className="tag-default tag-pill tag-outline">
             tag->s
           </li>,
         ),
     )
     |> React.array}
  </ul>;
};