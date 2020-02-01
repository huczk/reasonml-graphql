open ReactUtils;

[@react.component]
let make = (~slug) => {
  let {UserProvider.user} = UserProvider.useUser();
  let author = user->UserProvider.getUser;

  let (body, setBody) = React.useState(() => "");
  let randomId = Gql.randomId();

  let (mutation, simple, _full) =
    ApolloHooks.useMutation(
      ~variables=Comment_Query.CommentAdd.makeVariables(~slug, ~body, ()),
      ~optimisticResponse=
        Gql.makeOptimisticResponse({
          "__typename": "Mutation",
          "addComment":
            Comment_Query.createComment(~id=randomId, ~body, ~author),
        }),
      ~update=
        (client, _) =>
          Comment_Query.update(
            ~client,
            ~slug,
            ~updateData=
              Comment_Query.addToCache(~id=randomId, ~body, ~author),
          ),
      Comment_Query.CommentAdd.definition,
    );

  let isLoading =
    switch (simple) {
    | Loading => true
    | _ => false
    };

  <>
    {switch (user) {
     | Anonymous =>
       <p>
         <Link href="#/login"> "Sign in"->s </Link>
         " or "->s
         <Link href="#/register"> "sign up"->s </Link>
         " to add comments on this article."->s
       </p>
     | LoggedIn({image}) =>
       <form
         className="card comment-form"
         onSubmit={eventPrevent(_ =>
           mutation()
           |> Js.Promise.then_(_ => setBody(_ => "") |> Js.Promise.resolve)
           |> ignore
         )}>
         <div className="card-block">
           <textarea
             className="form-control"
             placeholder="Write a comment..."
             rows=3
             disabled=isLoading
             value={isLoading ? "" : body}
             onChange={eventPrevent(e => e |> eventTargetValue |> setBody)}
           />
         </div>
         <div className="card-footer">
           <img src=image className="comment-author-img" />
           <button className="btn btn-sm btn-primary">
             "Post Comment"->s
           </button>
         </div>
       </form>
     }}
  </>;
};