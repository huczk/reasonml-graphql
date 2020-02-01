module CommentFragment = [%graphql
  {|
     fragment Fragment on Comment {
      id
      body
      createdAt @bsDecoder(fn: "Gql.decodeDate")
      author {
        id
        image
        username
        __typename
      }
      __typename
    }
  |}
];

module CommentAdd = [%graphql
  {|
    mutation addComment($body: String!, $slug: String!) {
      addComment(body: $body, slug: $slug) {
        ...CommentFragment.Fragment
      }
    }
  |}
];

module CommentDelete = [%graphql
  {|
    mutation deleteComment($id: String!) {
      deleteComment(id: $id) {
        success
      }
    }
  |}
];

module GetComments = [%graphql
  {|
    query getComments($slug: String!) {
      comments(slug: $slug) {
        ...CommentFragment.Fragment
      }
    }
  |}
];

module AddCommentReadQuery = ApolloClient.ReadQuery(GetComments);
module AddCommentWriteQuery = ApolloClient.WriteQuery(GetComments);

external cast: Js.Json.t => GetComments.t = "%identity";

let createComment = (~id, ~body, ~author: UserProvider.loggedUser) => {
  "__typename": "Comment",
  "id": id,
  "body": body,
  "createdAt": Gql.encodeDate(),
  "author": {
    "id": author.id,
    "image": author.image,
    "username": author.username,
    "__typename": "Profile",
  },
};

let removeFromCache = (~id, ~cachedData) => {
  "comments":
    cachedData##comments
    |> Js.Array.filter(
         fun
         | Some(comment) =>
           String.compare(comment##id, id) == 0 ? false : true
         | None => false,
       ),
};

let addToCache = (~id, ~body, ~author, ~cachedData) => {
  "comments":
    cachedData##comments
    |> Js.Array.concat([|Some(createComment(~id, ~body, ~author))|]),
};

let update = (~client, ~slug, ~updateData) => {
  let query = GetComments.make(~slug, ());
  let readQueryOptions = ApolloHooks.toReadQueryOptions(query);

  switch (AddCommentReadQuery.readQuery(client, readQueryOptions)) {
  | exception _ => ()
  | cachedResponse =>
    switch (cachedResponse |> Js.Nullable.toOption) {
    | None => ()
    | Some(cachedPersons) =>
      AddCommentWriteQuery.make(
        ~client,
        ~variables=query##variables,
        ~data=updateData(~cachedData=cast(cachedPersons)),
        (),
      )
    }
  };
};