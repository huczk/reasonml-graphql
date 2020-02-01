open ReactUtils;

[@react.component]
let make = (~onTagClick=ignore) => {
  let (simple, _full) = ApolloHooks.useQuery(Tag_Query.GetTags.definition);

  <>
    <p> "Popular Tags"->s </p>
    <div className="tag-list">
      {switch (simple) {
       | Loading => "Loading..."->s
       | Data(data) =>
         data##tags
         ->Belt.Array.keepMap(
             fun
             | None => None
             | Some(tag) =>
               Some(
                 <Link
                   key=tag
                   href=""
                   className="tag-pill tag-default"
                   onClick={eventPrevent(_ => onTagClick(tag))}>
                   tag->s
                 </Link>,
               ),
           )
         ->React.array
       | _ => React.null
       }}
    </div>
  </>;
};